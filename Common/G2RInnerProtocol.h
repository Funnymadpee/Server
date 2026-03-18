#ifndef G2R_INNER_PROTOCOL_H
#define G2R_INNER_PROTOCOL_H

#include <cstdint>

// ==============================
// 【多Gate核心】
// 每个网关有唯一固定的 GateID
// ==============================
#define GATE1_ID 1
#define GATE2_ID 2
#define GATE3_ID 3


// Gate <-> Router 消息类型
#define MSG_CLIENT_PACKET    1  // 客户端数据包
#define MSG_CLIENT_DISCONNECT 2 // 客户端断开


// 客户端协议（仅Router解析）
#define PROTO_LOGIN_REQ 1001
#define PROTO_SCENCE_MOVE 8001      //场景移动协议


// Gate <-> Router 内部转发头
#pragma pack(push, 1)
struct InnerHead_Gate2Router {
    uint8_t  msgType;       ///消息类型
    uint32_t gateId;     // 网关ID（关键！区分哪个Gate）
    uint64_t connId;     // 连接ID（Gate内部唯一）
    uint16_t protoId;       //协议id
    uint16_t dataLen;    // 客户端包长度
};
#pragma pack(pop)

// ==============================
// Router 生成全局唯一Key
// ==============================
inline uint64_t MakeGlobalKey(uint32_t gateId, uint64_t connId) {
    return ((uint64_t)gateId << 32) | (connId & 0xFFFFFFFF);
}

#endif