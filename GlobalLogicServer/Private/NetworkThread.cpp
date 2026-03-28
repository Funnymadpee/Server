#include "../Public/NetworkThread.h"
#include "../Public/MessageQueue.h"
#include <iostream>
using namespace std;

NetworkThread& NetworkThread::instance() {
    static NetworkThread ins;
    return ins;
}

NetworkThread::NetworkThread()
    : _acceptor(_io, tcp::endpoint(tcp::v4(), 0))
{}

void NetworkThread::start(uint16_t port) {
    tcp::endpoint ep(boost::asio::ip::address_v4::any(), port);
    if (_acceptor.is_open()) {
        _acceptor.close();
    }
    _acceptor.open(ep.protocol());
    //允许端口复用
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(ep);
    _acceptor.listen();
    _thread = std::thread(&NetworkThread::run, this);
    cout << "[网络线程] 启动成功，监听端口:" << port << endl;
}

void NetworkThread::run() {
    startAccept();
    _io.run();
}

void NetworkThread::startAccept() {
    auto sock = make_shared<tcp::socket>(_io);
    _acceptor.async_accept(*sock, [this, sock](const boost::system::error_code& ec) {
        if (!ec) readSession(sock); //开始读数据
        startAccept();
    });
}

void NetworkThread::readSession(std::shared_ptr<tcp::socket> sock) {
    auto headBuf = make_shared<vector<char>>(sizeof(InnerHead));
    boost::asio::async_read(*sock, boost::asio::buffer(*headBuf),
        [this, sock, headBuf](const boost::system::error_code& ec, size_t) {
            if (ec) return;

            InnerHead head = *(InnerHead*)headBuf->data();
            auto bodyBuf = make_shared<vector<char>>(head.dataLen);

            boost::asio::async_read(*sock, boost::asio::buffer(*bodyBuf),
                [this, sock, head, bodyBuf](const boost::system::error_code& ec, size_t) {
                    if (ec) return;

                    // 生产消息 压入队列
                    LogicMessage* msg = new LogicMessage();
                    msg->head = head;
                    msg->body = *bodyBuf;
                    MessageQueue::instance().push(msg);
                    readSession(sock);
                });
        });
}