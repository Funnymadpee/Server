#include "../Public/Router.h"
#include <iostream>
using namespace std;


void Router::start(boost::asio::io_context& io)
{
    // 创建场景服连接
    auto scene1 = ServerConnection::create(io, "127.0.0.1", 8001, 1);
    m_sceneServers[1] = scene1;
    scene1->start();


}

///获取场景服连接
ServerConnection* Router::getSceneServerById(int serverId)
{
    auto it = m_sceneServers.find(serverId);
    if (it != m_sceneServers.end()) 
    {
        auto& srv = it->second;
        if (srv && srv->isConnected()) 
        {
            return srv.get();
        }
    }
    
    return nullptr;
}


//网络线程
void Router::pushMessage(const InnerHead_Gate2Router* head, const char* body) {
    RouterMessage msg;
    msg.gateId = head->gateId;
    msg.connId = head->connId;
    msg.msgType = head->msgType;
    msg.protoId = head->protoId;

    //处理客户端断开连接的消息  //直接处理  datalen=0
    if (msg.msgType == MSG_CLIENT_DISCONNECT) {
        onClientDisconnect(msg.gateId, msg.connId);
    }

    if (body && head->dataLen > 0) {
        msg.body.resize(head->dataLen);
        memcpy(msg.body.data(), body, head->dataLen);
    }

    msgQueue_.push(std::move(msg));
}

//逻辑线程
void Router::update() {
    RouterMessage msg;
    while (msgQueue_.pop(msg)) {
        processMessage(msg);
    }
}

void Router::processMessage(const RouterMessage& msg) {
    if (msg.msgType == MSG_CLIENT_DISCONNECT) {
        onClientDisconnect(msg.gateId, msg.connId);
    }
    else if (msg.msgType == MSG_CLIENT_PACKET) {
        onClientPacket(msg.gateId, msg.connId, msg.protoId,msg.body.data(), (uint16_t)msg.body.size());
    }
}

// ==============================
// 解析客户端协议  协议id+数据
void Router::onClientPacket(uint32_t gateId, uint64_t connId,uint16_t protoId, const char* data, uint16_t len)
{
    if (len < 2) return;

    uint16_t Id = protoId;
    cout << "[Router] 解析客户端协议 protoId=" << Id << endl;

    //假登录
    if (Id == PROTO_LOGIN_REQ)
    {
        uint64_t globalKey = MakeGlobalKey(gateId, connId);
        uint64_t uid = 10000 + (globalKey % 10000);
        bindUid(globalKey, uid);
    }
    //查路由表
    ServerType target = RouteTable::instance().getTargetServer(protoId);
    switch (target)
    {
    case ServerType::SceneServer:
        //是游戏场景服内消息 查找是否之前绑定过
        //获取uid == Playerid    这边暂时临时处理
        uint64_t globalKey = MakeGlobalKey(gateId, connId);
        uint64_t uid = 10000 + (globalKey % 10000);
        int serverid;
        bool hasBind = PlayerRouter::instance().getPlayerServer(uid, target, serverid);
        //如果已经绑定 直接转发  没有就转发到默认场景服
        if(hasBind)
        {
            forwardToScene(uid, protoId, serverid, data, len);
            return;
        }
        else
        {
            int defaultserverid = 1;
            forwardToScene(uid, protoId, defaultserverid, data, len);
        }
        break;
    
    default:
        break;
    }
}

//断联清理
void Router::onClientDisconnect(uint32_t gateId, uint64_t connId) {
    uint64_t globalKey = MakeGlobalKey(gateId, connId);
    cout << "[Router] 客户端断开 globalKey=" << globalKey << endl;
    auto it = globalMap.find(globalKey);
    if (it != globalMap.end()) {
        uint64_t uid = it->second.uid;
        if (uid != 0) {
            uidToGlobalKey.erase(uid);
            cout << "[Router] 清理 UID=" << uid << endl;
        }
        globalMap.erase(it);

    }
    
}


// 绑定 UID 到 GlobalKey
void Router::bindUid(uint64_t globalKey, uint64_t uid)
{
    // 1. 把 uid 存到会话里
    globalMap[globalKey].uid = uid;

    // 2. 建立 uid -> GlobalKey 的映射
    uidToGlobalKey[uid] = globalKey;

    cout << "[Router] 绑定成功 UID=" << uid 
         << "  GlobalKey=" << globalKey << endl;
}

//转发到场景服
void Router::forwardToScene(
    uint64_t playerId,
    uint16_t serverid,
    uint16_t protoId, const char* data, uint16_t len)
{
    ServerConnection* svr = getSceneServerById(serverid);
    if (svr) {
        svr->send( playerId,serverid, protoId, data, len);
    }
}