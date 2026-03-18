#pragma once
#include "GlobalLogicDef.h"
#include "ServerConnection.h"
#include "NatsClient.h"
#include "MessageQueue.h"
#include "../../Common/InnerProtocol.h"

class LogicDispatcher {
public:
    static LogicDispatcher& instance() {
        static LogicDispatcher ins;
        return ins;
    }

    // 初始化连接 Router
    void initRouterConn(boost::asio::io_context& io);

    // 发给 Router：绑定玩家到场景
    void sendBindPlayer(uint64_t playerId, int sceneId);

    // 发给 Router：解绑玩家
    void sendUnbindPlayer(uint64_t playerId);
    ///////////////////////////////////////////////////////
    //注册协议
    void init();
    //协议分发
    void dispatch(LogicMessage* msg);

    /////////////////////////////////////////////////////////
    //微服务
    void callBagService(uint64_t playerId);


    ///////////////////////////////////////////////////////
    void update();

private:

    LogicDispatcher() = default;

    // 协议映射表：protoId → 处理函数
    using Handler = std::function<void(LogicMessage*)>;
    std::unordered_map<uint16_t, Handler> _handlers;

    void handleClientMsg(LogicMessage* msg);    // 客户端消息



    // 全局逻辑服 <-> Router 连接（放在这里最正确）
    ServerConnection::Pointer _routerConn;

    uint64_t _nowTime;
};