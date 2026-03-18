#ifndef GATESERVER_H
#define GATESERVER_H

#include <boost/asio.hpp>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include "Session.h"
#include "GateRouterClient.h"

using boost::asio::ip::tcp;

class GateServer {
public:
    GateServer(boost::asio::io_context& ioc, uint32_t gateId);

    void start();
    GateRouterClient* routerClient() { return &routerClient_; }
    uint32_t GetGateId() {return gateId_;}

    //发给router
    void sendToRouter(uint32_t gateId,uint64_t connId,uint8_t msgType,
    uint16_t protoId,
    const char* body,
    uint16_t dataLen
    );


private:
    void doAccept();
    uint64_t makeConnId() { return ++connIdSeed_; } //连接id

private:
    tcp::acceptor acceptor_;
    boost::asio::io_context& ioc_;
    uint64_t connIdSeed_ = 0;
    std::unordered_map<uint64_t, std::shared_ptr<Session>> sessionMap_;
    uint32_t gateId_;
    GateRouterClient routerClient_;
};

#endif