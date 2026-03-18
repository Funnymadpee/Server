#include "../Public/GateRouter.h"
#include <iostream>
#include <vector>
using namespace std;

GateRouter::GateRouter(boost::asio::io_context& ioc, int port, Router& router)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)),
      router_(router)
{
    router.start(ioc);
    doAccept();
}

void GateRouter::doAccept() {
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            //连接接入
            startGateSession(std::move(socket));
        }
        doAccept();
    });
}

void GateRouter::startGateSession(tcp::socket socket) {
    auto sock = make_shared<tcp::socket>(std::move(socket));
    auto head = make_shared<InnerHead_Gate2Router>();

    //先读消息头
    boost::asio::async_read(*sock,
        boost::asio::buffer(head.get(), sizeof(InnerHead_Gate2Router)),
        [this, sock, head](boost::system::error_code ec, size_t) {
            if (ec) return;
            if (head->dataLen > 0) {
                // 读客户端包体
                auto body = make_shared<vector<char>>(head->dataLen);

                boost::asio::async_read(*sock,
                    boost::asio::buffer(*body),
                    [this,sock, head, body](boost::system::error_code ec2, size_t)
                    {
                        if (!ec2)
                        {
                            // 头 + 体 一起交给 Router管理
                            router_.pushMessage(head.get(), body->data());
                        }
                        // 读完继续监听下一条消息
                        startGateSession(move(*sock));
                    });
            }
            else
            {
                router_.pushMessage(head.get(), nullptr);
                startGateSession(move(*sock));
            }
        });
}