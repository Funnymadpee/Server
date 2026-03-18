#include "../Public/MessageQueue.h"

MessageQueue& MessageQueue::instance() {
    static MessageQueue ins;
    return ins;
}

void MessageQueue::push(SceneMessage* msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(msg);
    _cv.notify_one();           //信号量统治 防止cpu空转
}

SceneMessage* MessageQueue::pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() { return !_queue.empty(); });

    SceneMessage* msg = _queue.front();
    _queue.pop();
    return msg;
}