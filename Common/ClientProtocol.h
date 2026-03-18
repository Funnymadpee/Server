#pragma once

#include <stdint.h>

// 客户端 -> Gate 消息头（解决粘包）
struct ClientHeader {
    uint16_t len;       // 总长度 = header + body
    uint16_t protoId;   // 协议ID
};

// 客户端 ↔ Gate 唯一需要的协议：心跳
#define PROTO_HEARTBEAT    1000