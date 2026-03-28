#pragma once
#include <map>
#include "GlobalLogicDef.h"
#include "ServerConnection.h"
#include "NatsClient.h"
#include "MessageQueue.h"
#include "../../Common/InnerProtocol.h"



#define SceneServer1_ID 1   //场景服1   8801
#define SceneServer2_ID 2   //场景服2   8802


class LogicDispatcher {
public:
    static LogicDispatcher& instance() {
        static LogicDispatcher ins;
        return ins;
    }

    // 初始化连接 Router  scene
    void initRouterConn(boost::asio::io_context& io);

    void initSceneConn(boost::asio::io_context& io, int serverid, int port);



    // 发给 Router：绑定玩家到场景
    void sendBindPlayer(uint64_t playerId, int serverid, int sceneId);
    // 发给 Router：解绑玩家
    void sendUnbindPlayer(uint64_t playerId);


    //发给scene 生成角色
    void sendCreatePlayer(LogicMessage* msg);



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


    boost::asio::io_context ioc;
    // 全局逻辑服 <-> Router 连接
    ServerConnection::Pointer _routerConn;

    //全局逻辑服 -- 场景服id 场景服连接
    std::map<int, ServerConnection::Pointer> _SceneConns;

    uint64_t _nowTime;
};