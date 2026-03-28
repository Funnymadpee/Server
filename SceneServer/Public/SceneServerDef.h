#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "../../Common/InnerProtocol.h"

// 消息封装结构
#pragma pack(push, 1)
struct SceneMessage {
    InnerHead head;
    char      body[1024];
};
#pragma pack(pop)

struct LocData {
    int loc_x;
    int loc_y;
};