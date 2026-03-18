#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "../../Common/InnerProtocol.h"

// 消息封装结构
struct LogicMessage {
    InnerHead head;
    std::vector<char> body;
};