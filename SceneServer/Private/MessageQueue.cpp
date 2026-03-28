#include "../Public/MessageQueue.h"

MessageQueue& MessageQueue::instance() {
    static MessageQueue ins;
    return ins;
}

void MessageQueue::push(SceneMessage* msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(msg);

}

SceneMessage* MessageQueue::pop() {
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