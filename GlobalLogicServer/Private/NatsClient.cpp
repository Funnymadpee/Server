#include "../Public/NatsClient.h"
#include <nats/nats.h>
#include <cstring>
#include <stdexcept>

//回调函数
static std::function<void(const InnerHead&, const char*)> g_callback;

NatsClient& NatsClient::instance()
{
    static NatsClient ins;
    return ins;
}

NatsClient::NatsClient()
    : _natsConn(nullptr)
    , _connected(false)
{
}

NatsClient::~NatsClient()
{
    if (_natsConn) {
        natsConnection_Destroy((natsConnection*)_natsConn); //销毁链接
    }
}

bool NatsClient::connect(const std::string& ip, int port)
{
    if (_connected) return true;

    std::string url = "nats://" + ip + ":" + std::to_string(port);
    natsConnection* conn = nullptr;
    natsOptions* opts = nullptr;

    //创建配置对象
    natsStatus s = natsOptions_Create(&opts);
    if (s != NATS_OK) {
        return false;
    }

    //配置设置服务器地址
    s = natsOptions_SetURL(opts, url.c_str());
    if (s != NATS_OK) {
        natsOptions_Destroy(opts);
        return false;
    }

    // 绑定配置
    s = natsConnection_Connect(&conn, opts);
    if (s != NATS_OK) {
        natsOptions_Destroy(opts);
        return false;
    }

    //销毁配置对象 连接已建立，不再需要
    natsOptions_Destroy(opts);

    _natsConn = conn;
    _connected = true;
    return true;
}
// 全局逻辑服 -> 中间件 -> 微服务   
void NatsClient::publish(const std::string& subject, const InnerHead& head, const char* body)
{
    if (!_connected) return;

    // 消息 = Head + Body
    int totalLen = sizeof(InnerHead) + head.dataLen;
    char* buf = new char[totalLen];

    memcpy(buf, &head, sizeof(InnerHead));
    if (body && head.dataLen > 0) {
        memcpy(buf + sizeof(InnerHead), body, head.dataLen); //消息体
    }

    natsConnection_Publish((natsConnection*)_natsConn,
        subject.c_str(),
        (const void*)buf, totalLen);

    delete[] buf;
}

// 微服务订阅接收
static void onMsgReceived(natsConnection* nc, natsSubscription* sub,
    natsMsg* msg, void* closure)
{
    const void* data = natsMsg_GetData(msg);
    size_t len = natsMsg_GetDataLength(msg);

    if (len < sizeof(InnerHead)) {
        natsMsg_Destroy(msg);
        return;
    }

    InnerHead head = *(const InnerHead*)data;
    const char* body = (const char*)data + sizeof(InnerHead);

    if (g_callback) {
        g_callback(head, body);
    }

    natsMsg_Destroy(msg);
}

void NatsClient::subscribe(const std::string& subject,
    std::function<void(const InnerHead& head, const char* body)> cb)
{
    if (!_connected) return;

    //设置回调函数
    g_callback = cb;

    natsSubscription* sub = nullptr;
    natsConnection_Subscribe(&sub, (natsConnection*)_natsConn,
        subject.c_str(), onMsgReceived, nullptr);
}