#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "GlobalLogicDef.h"

class MessageQueue {
public:
    static MessageQueue& instance();

    // 网络线程写入
    void push(LogicMessage* msg);

    //逻辑线程读取
    LogicMessage* pop();

private:
    MessageQueue() = default;

    std::queue<LogicMessage*>  _queue;
    std::mutex                  _mutex;
    std::condition_variable     _cv;
};