#include "../Public/GateServer.h"
#include <iostream>

using namespace std;
#define PORT 8888

GateServer::GateServer(boost::asio::io_context& ioc,uint32_t gateId)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), PORT)),
      ioc_(ioc), gateId_(gateId),routerClient_(ioc)
{
    connIdSeed_ = 0;
}

void GateServer::start() {
    // 连接 Router  这边暂时没用服务发现 router的端口是9999
    routerClient_.connect("127.0.0.1", 9999);
    doAccept();
}

void GateServer::doAccept() {
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            uint64_t connId = makeConnId();
            cout << "新客户端连接 connId=" << connId << endl;

            auto session = make_shared<Session>(move(socket), ioc_, *this, connId);
            sessionMap_[connId] = session;
            session->start();
        }
        doAccept();
    });
}

void GateServer::sendToRouter(
    uint32_t gateId,
    uint64_t connId,
    uint8_t msgType,
    uint16_t protoId,
    const char* body,
    uint16_t dataLen
) {
    InnerHead_Gate2Router head{};
    head.gateId = gateId;
    head.connId = connId;
    head.msgType = msgType;
    head.protoId = protoId;
    head.dataLen = dataLen;

    routerClient_.send(
        (const char*)&head, sizeof(head),
        body, dataLen
    );
}