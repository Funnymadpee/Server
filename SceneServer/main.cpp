#include "./Public/Scene/SceneManager.h"
#include "./Public/Scene/SceneDispatcher.h"
#include "./Public/Network/NatsClient.h"
#include "./Public/MessageQueue.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <cstring>
#include <vector>

#include "./Public/Network/NetworkThread.h"

const int FRAME_MS = 50;        //服务器帧数
const int MAX_PROCESS_MSG = 10;     //网络每帧最大处理消息数量

uint64_t getNowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

void update(int delta)
{
    // 场景帧更新
    SceneManager::instance().update(delta);
}

int main() {

    const uint16_t PORT = 9999;

    //初始化场景管理
    SceneManager::instance().init();
    SceneDispatcher::instance().init();

    //启动网络线程
    NetworkThread::instance().start(PORT);

    //连接NATS中间件
    NatsClient::instance().connect();

    //监听中间件消息
    NatsClient::instance().subscribe("service.scene.1001", [](const InnerHead& head, const char* body) {
        SceneMessage msg{};
        msg.head = head;
        msg.body.assign(body, body + strlen(body));
        MessageQueue::instance().push(&msg);
    });

    while (true) {

        uint64_t now = getNowMs();
        int processed = 0;
        //每帧固定处理消息的次数  将消息从缓冲区弹出 分发到具体区域的消息队列中
        while (processed < MAX_PROCESS_MSG)
        {
            SceneMessage* msg = MessageQueue::instance().pop();
            SceneDispatcher::instance().dispatch(msg);
            delete msg;
            processed++;
        }

        //世界更新
        uint64_t delta = getNowMs() - now;
        update(delta);

        // 不忙的时候休眠 稳定服务器tick
        if (delta < FRAME_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_MS - delta));
        }
    }
    return 0;
}
