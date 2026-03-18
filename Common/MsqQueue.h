#include <mutex>
#include <queue>
#include <vector>

//路由消息结构
struct RouterMessage {
    uint32_t gateId;
    uint64_t connId;
    uint8_t msgType;
    uint16_t protoId;
    std::vector<char> body;
};

//路由消息队列
class MsgQueue {
public:
    void push(RouterMessage&& msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(std::move(msg));
    }

    bool pop(RouterMessage& out) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) return false;
        out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

private:
    std::queue<RouterMessage> queue_;
    std::mutex mtx_;
};
