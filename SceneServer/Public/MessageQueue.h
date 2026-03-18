#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "SceneServerDef.h"

class MessageQueue {
public:
    static MessageQueue& instance();

    // 网络线程写入
    void push(SceneMessage* msg);

    //逻辑线程读取
    SceneMessage* pop();

private:
    MessageQueue() = default;

    std::queue<SceneMessage*>  _queue;
    std::mutex                  _mutex;
    std::condition_variable     _cv;
};