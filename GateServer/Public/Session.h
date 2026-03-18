#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include "HeartBeat.h"
#include "../../Common/ClientProtocol.h"
#include "../../Common/G2RInnerProtocol.h"

using boost::asio::ip::tcp;

class GateServer;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, boost::asio::io_context& ioc, 
            GateServer& gateServer, uint64_t connId);
    void start();
    void close();
    uint64_t connId() const { return connId_; }
    void readClientPacket();  // 读客户端完整包

private:
    void readLoop();
    void onMessage(const std::vector<char>& body);

private:
    std::shared_ptr<tcp::socket> socket_;
    HeartBeat heartBeat_;
    GateServer& gateServer_;
    ClientHeader clientHeader_; //客户端协议头
    uint64_t connId_;  // 唯一连接ID
    uint32_t gateId_;  // 自己的GateID
    bool isClosed_ = false;  //关闭标记
};

#endif