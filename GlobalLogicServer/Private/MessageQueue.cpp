#include "../Public/MessageQueue.h"

MessageQueue& MessageQueue::instance() {
    static MessageQueue ins;
    return ins;
}

void MessageQueue::push(LogicMessage* msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(msg);
    _cv.notify_one();           //信号量统治 防止cpu空转
}

LogicMessage* MessageQueue::pop() {
    std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
    if (lock) {
        while (!_queue.empty()) {
            auto msg = std::move(_queue.front());
            _queue.pop();
            //消息
            return msg;
        }
    }
    return nullptr;
}