#include "../Public/HeartBeat.h"

HeartBeat::HeartBeat(boost::asio::io_context& ioc, int timeoutSeconds)
    : timer_(ioc), timeout_sec_(timeoutSeconds), isActive_(false) {}

void HeartBeat::setOnTimeout(TimeoutCallback callback) {
    onTimeout_ = std::move(callback);
}

void HeartBeat::start() {
    isActive_ = true;
    lastActiveTime_ = steady_timer::clock_type::now();
    checkTimer();
}

void HeartBeat::refresh() {
    lastActiveTime_ = steady_timer::clock_type::now();
}

void HeartBeat::stop() {
    isActive_ = false;
    timer_.cancel();
}

void HeartBeat::checkTimer() {
    if (!isActive_) return;

    auto now = steady_timer::clock_type::now();
    auto dur = duration_cast<seconds>(now - lastActiveTime_);

    if (dur.count() >= timeout_sec_) {
        if (onTimeout_) onTimeout_();
        return;
    }

    timer_.expires_after(seconds(1));
    timer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) checkTimer();
    });
}