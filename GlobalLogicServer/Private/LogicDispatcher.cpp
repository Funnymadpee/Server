#include"../Public/LogicDispatcher.h"
#include <iostream>
using namespace std;

    void LogicDispatcher::init() {
        // 客户端转发消息协议注册
        _handlers[PROTO_FORWARD_MSG] = [this](LogicMessage* msg) {
            handleClientMsg(msg);
        };
        _handlers[0x3001] = [this](LogicMessage* msg) {
            //背包服务
            uint64_t playerId = msg->head.playerId;
            callBagService(playerId);
        };
    }

    void LogicDispatcher::dispatch(LogicMessage* msg){
        cout << "[主线程逻辑] 玩家ID:" << msg->head.playerId
             << " 协议ID:" << msg->head.protoId << endl;

        auto it = _handlers.find(msg->head.protoId);
        if (it != _handlers.end()) {
            it->second(msg); // 直接调用处理函数
        } else {
            printf("未知协议ID: %d\n", msg->head.protoId);
        }

        // 在这里写你的全局逻辑：
        // 玩家管理、场景分配、同步Router、微服务调用...
    }

////////////////////////////////////////////////////////////////////////
    // 初始化连接 Router
    void LogicDispatcher::initRouterConn(boost::asio::io_context& io) {
        // 复用你的 ServerConnection 连接 Router
        _routerConn = ServerConnection::create(io, "127.0.0.1", 8888, 0);
        _routerConn->start();
    }

////////////////////////////////////////////////////////////////////////
    // 发给 Router：绑定玩家到场景
    void LogicDispatcher::sendBindPlayer(uint64_t playerId, int sceneId) {
        //第二个参数暂时瞎写
        _routerConn->send(playerId, 0,0x1001,(const char*)&sceneId, sizeof(sceneId));
    }

    // 发给 Router：解绑玩家
    void LogicDispatcher::sendUnbindPlayer(uint64_t playerId) {
        _routerConn->send(playerId, 0x1002);
    }


/////////////////////////////////////////////////////////////////////
    //走微服务
    //
    void LogicDispatcher::handleClientMsg(LogicMessage* msg) {
        //相关处理
    }

    // 调用背包微服务
    void LogicDispatcher::callBagService(uint64_t playerId) {
        InnerHead head{};
        head.playerId = playerId;
        head.protoId = 0x3001; //回复是0x3002
        head.dataLen = 0;
        NatsClient::instance().publish("service.bag", head, nullptr);
    }

    // nat监听微服务回包 订阅回复
    void listenReply() {
    NatsClient::instance().subscribe("reply.global", [](const InnerHead& head, const char* body) {
        printf("全局服收到回包: player=%llu, proto=%d\n", head.playerId, head.protoId);
        //再组成消息包 放到主线程消息队列里
        //构造消息
        LogicMessage* msg = new LogicMessage();
        msg->head = head;
        msg->body.assign(body, body + head.dataLen);

       // 2. 扔给主线
        MessageQueue::instance().push(msg);
    
    });


    }
//////////////////////////////////////////////////////////////////////
//世界更新
void LogicDispatcher::update()
{
    // 获取当前时间
    // 1. 更新所有玩家在线状态
    // 2. 清理超时玩家
    // 3. 定时器检查
    // 4. 同步数据到Router/微服务
    // 5. 日志、统计、监控等
}
