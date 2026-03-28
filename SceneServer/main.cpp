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
    // 场景帧更新  投递场景更新任务
    SceneManager::instance().update(delta);
    //场景区域的全局aoi更新
    //AoiManager::instance().update(delta);
}

int main() {
    // 解决 Windows 控制台中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const uint16_t PORT = 8801;

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
        const char* body_data = body;
        size_t body_len = strlen(body_data);

        if (body_len >= sizeof(msg.body)) {
            body_len = sizeof(msg.body) - 1;
        }
        memcpy(msg.body, body_data, body_len);
        msg.body[body_len] = '\0';

        SceneMessage* msg_ptr = new SceneMessage(msg); // 拷贝构造
        MessageQueue::instance().push(msg_ptr);
    });

    while (true) {

        uint64_t now = getNowMs();
        int processed = 0;
        //每帧固定处理消息的次数  将消息从缓冲区弹出 分发到具体区域的消息队列中
        while (processed < MAX_PROCESS_MSG)
        {
            SceneMessage* msg = MessageQueue::instance().pop();
            if(msg == nullptr)
                break;
            SceneDispatcher::instance().dispatch(msg);
            delete msg;
            processed++;
        }

        //世界更新
        uint64_t delta = getNowMs() - now;
        update(delta);
        
    }
    return 0;
}
