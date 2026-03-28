#ifndef G2R_INNER_PROTOCOL_H
#define G2R_INNER_PROTOCOL_H

#include <cstdint>

// 客户端协议（仅Router解析）
#define PROTO_LOGIN_REQ 1001
#define PROTO_SCENCE_MOVE 8001      //场景移动协议


//客户端协议
#define PROTO_FORWARD_MSG 7001


// Router 内部转发头
#pragma pack(push, 1)
struct InnerHead {
    uint16_t protoId;       //协议id
    int playerId;           //玩家uid
    int ServerId;           //服务器id
    int SceneId;            //场景id
    uint16_t dataLen;    // 客户端包长度
};
#pragma pack(pop)

#endif