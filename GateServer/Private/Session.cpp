#include "../Public/Session.h"
#include "../Public/GateServer.h"
#include "../Public/GateRouterClient.h"

#include <iostream>

using namespace std;

#define HEARTBEAT_TIMEOUT 30

Session::Session(tcp::socket socket, boost::asio::io_context& ioc, 
                 GateServer& gateServer, uint64_t connId)
    : socket_(make_shared<tcp::socket>(move(socket))),
      heartBeat_(ioc, HEARTBEAT_TIMEOUT),
      gateServer_(gateServer),
      connId_(connId)
{
    gateId_ = gateServer_.GetGateId();
    heartBeat_.setOnTimeout([this]() {
        cout << "[心跳超时] connId=" << connId_ << endl;
        close();
    });
}

void Session::start() {
    heartBeat_.start(); //开始心跳
    readClientPacket();
}

//读完整的数据包 然后路由到router
void Session::readClientPacket() {
    auto self = shared_from_this();
    auto sock = socket_;

    // 1. 读消息头  len  协议id
    boost::asio::async_read(*sock,
        boost::asio::buffer(&clientHeader_, sizeof(ClientHeader)),
        [this, self, sock](boost::system::error_code ec, size_t) {
            if (ec) {
                close();
                return;
            }

            uint16_t bodyLen = clientHeader_.len - sizeof(ClientHeader);
            if (bodyLen > 1024 * 1024) { // 防攻击
                close();
                return;
            }

            // 2. 读包体 
            auto bodyBuf = make_shared<vector<char>>(bodyLen);
            boost::asio::async_read(*sock,
                boost::asio::buffer(*bodyBuf, bodyLen),
                [this, self, sock, bodyBuf](boost::system::error_code ec2, size_t) {
                    if (ec2) {
                        close();
                        return;
                    }

                    // 收到完整包 → 处理心跳 OR 转发给 Router
                    if (clientHeader_.protoId == PROTO_HEARTBEAT) {
                        // 心跳：只刷新，不转发
                        heartBeat_.refresh();
                    } else {
                        // 业务包 封装gateid  connid 客户端包type 协议id 包data 大小
                        gateServer_.sendToRouter(
                            gateId_,
                            connId_,
                            MSG_CLIENT_PACKET,
                            clientHeader_.protoId,
                            bodyBuf->data(),
                            bodyBuf->size()
                        );
                    }

                    // 继续读下一条消息
                    readClientPacket();
                });
        });
}


// 这边不解析协议 放到router解析 保证gate无状态集群
// void Session::onMessage(const vector<char>& body) {
//     cout << "[Gate] 收到客户端消息，转发给 Router | connId=" << connId_ << endl;

//     // 组内部协议头
//     InnerHead_Gate2Router innerHead;
//     innerHead.msgType   = MSG_CLIENT_PACKET;
//     innerHead.connId = connId_;
//     innerHead.gateId = gateId_;
//     innerHead.dataLen = (uint16_t)body.size();

//     cout << "[Gate] 转发到 Router | gateId=" << gateId_
//          << " connId=" << connId_ << endl;

//     int totalSize = sizeof(InnerHead_Gate2Router) + body.size();
//     vector<char> sendBuf(totalSize);

//     memcpy(sendBuf.data(), &innerHead, sizeof(InnerHead_Gate2Router));
//     memcpy(sendBuf.data() + sizeof(InnerHead_Gate2Router), body.data(), body.size());

//     // 这里发送给 Router
//     gateServer_.routerClient()->send(sendBuf.data(), sendBuf.size());

// }

void Session::close() {

    if (isClosed_) {
        return;  // 已经关闭过，直接返回
    }
    isClosed_ = true;

    heartBeat_.stop();
    boost::system::error_code ec;
    cout << "[Gate] 连接关闭 connId=" << connId_ << endl;
    InnerHead_Gate2Router head{};
    head.msgType = MSG_CLIENT_DISCONNECT;
    head.gateId = gateId_;
    head.connId = connId_;
    head.dataLen = 0;
    gateServer_.routerClient()->send((const char*)&head, sizeof(head));
    cout << "[Gate] 客户端断开 connId=" << connId_ << " → 已通知Router清理" << endl;
    socket_->close(ec);
}