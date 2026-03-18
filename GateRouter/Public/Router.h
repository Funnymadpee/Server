#ifndef ROUTER_H
#define ROUTER_H

#include <unordered_map>
#include "../../Common/G2RInnerProtocol.h"
#include "../../Common/MsqQueue.h"
#include "RouteTable.h"
#include "PlayerRouter.h"
#include "ServerConnection.h"

class Router {
public:

    //网络线程处理网络消息压入队列
    void pushMessage(const InnerHead_Gate2Router* head, const char* body);

    // 逻辑线程处理队列中的消息
    void update();

    //对业后面务服务器的连接
    void start(boost::asio::io_context& io);

    //获取场景服的连接
    ServerConnection* getSceneServerById(int serverId);


private:
    void processMessage(const RouterMessage& msg);
    void onClientPacket(uint32_t gateId, uint64_t connId, uint16_t protoId, const char* data, uint16_t len);
    void onClientDisconnect(uint32_t gateId, uint64_t connId);
    void bindUid(uint64_t globalKey, uint64_t uid);

    //路由转发给指定场景服
    void Router::forwardToScene(
    uint64_t playerId,
    uint16_t serverid,
    uint16_t protoId, const char* data, uint16_t len);

private:
    struct ClientSession {
        uint64_t uid = 0;
    };

    MsgQueue msgQueue_;

    //玩家uid绑定gate
    std::unordered_map<uint64_t, ClientSession> globalMap;
    std::unordered_map<uint64_t, uint64_t> uidToGlobalKey;

    //玩家绑定后面服务器  这个放在在PlayerRouter中管理

    //router绑定业务服务器
    std::unordered_map<int, ServerConnection::Pointer> m_sceneServers;      //游戏场景服务器
    std::unordered_map<int, ServerConnection::Pointer> m_logicServers;      //游戏逻辑服务器 现在就一个全局
    std::unordered_map<int, ServerConnection::Pointer> m_MicroServers;      //微服务服务器

};

#endif