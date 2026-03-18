#pragma once
#include <string>
#include <functional>
#include "../SceneServerDef.h"
#include "../../../Common/InnerProtocol.h"

class NatsClient
{
public:
    static NatsClient& instance();

    // 连接中间件
    bool connect(const std::string& ip = "127.0.0.1", int port = 4222);

    // 发布消息
    void publish(const std::string& subject, const InnerHead& head, const char* body);

    // 订阅消息
    void subscribe(const std::string& subject,
        std::function<void(const InnerHead& head, const char* body)> cb);

private:
    NatsClient();
    ~NatsClient();

    void* _natsConn; // NATS 连接句柄
    bool _connected = false;
};