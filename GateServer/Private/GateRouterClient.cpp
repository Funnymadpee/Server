#include "../Public/GateRouterClient.h"
#include <iostream>
using namespace std;
using boost::asio::ip::tcp;

GateRouterClient::GateRouterClient(boost::asio::io_context& ioc) : sock(ioc) {}

void GateRouterClient::connect(const char* ip, int port) {
    tcp::resolver r(sock.get_executor());
    auto ep = r.resolve(ip, to_string(port));
    boost::asio::async_connect(sock, ep, [this](auto ec, auto) {
        if (!ec) connected = true;
    });
}

void GateRouterClient::send(const char* data, int len) {
    if (!connected) {
        cout << "[GateRouterClient] 未连接 Router，发送失败！" << endl;
        return;
    }

    cout << "[GateRouterClient] 发送消息到 Router，len=" << len << endl;
    
    
    if (connected)
        boost::asio::async_write(sock, boost::asio::buffer(data, len), [](auto, auto) {});
}

void GateRouterClient::send(const char* headData, size_t headLen, 
                            const char* bodyData, size_t bodyLen) {
    if (!connected) {
        cout << "[GateRouterClient] 未连接 Router，发送失败！" << endl;
        return;
    }

    // 把 head 和 body 拼成一个 buffer 列表，一次性发送
    vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(headData, headLen));
    if (bodyData && bodyLen > 0) {
        buffers.push_back(boost::asio::buffer(bodyData, bodyLen));
    }

    cout << "[GateRouterClient] 发送消息到 Router，总长度=" 
         << (headLen + bodyLen) << endl;

    boost::asio::async_write(sock, buffers,
        [this](boost::system::error_code ec, size_t bytesSent) {
            if (ec) {
                cout << "[GateRouterClient] 发送失败: " << ec.message() << endl;
                //
                connected = false;
            } else {
                cout << "[GateRouterClient] 发送成功，bytes=" << bytesSent << endl;
            }
        });
}
