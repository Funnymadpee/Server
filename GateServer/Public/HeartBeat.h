#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <chrono>

using namespace std::chrono;
using boost::asio::steady_timer;

class HeartBeat {
public:
    using TimeoutCallback = std::function<void()>;

    explicit HeartBeat(boost::asio::io_context& ioc, int timeoutSeconds = 30);

    // 启动心跳检测
    void start();

    // 刷新心跳（收到消息时调用）
    void refresh();

    // 停止心跳
    void stop();

    // 设置超时回调（正确公有接口）
    void setOnTimeout(TimeoutCallback callback);

private:
    void checkTimer();

private:
    steady_timer timer_;
    int timeout_sec_;
    TimeoutCallback onTimeout_;  // 仍然是私有！
    bool isActive_;
    steady_timer::time_point lastActiveTime_;
};

#endif