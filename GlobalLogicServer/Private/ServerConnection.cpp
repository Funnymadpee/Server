#include "../Public/ServerConnection.h"
#include "../../Common/InnerProtocol.h"
#include <iostream>
using namespace std;

ServerConnection::ServerConnection(
    boost::asio::io_context& io, 
    const string& ip, int port, int serverId)
    : m_io(io)
    , m_socket(io)
    , m_ip(ip)
    , m_port(port)
    , m_serverId(serverId)
{
}

ServerConnection::~ServerConnection()
{
    if (m_socket.is_open()) {
        boost::system::error_code ec;
        m_socket.close(ec);
    }
}

void ServerConnection::start()
{
    doConnect();
}

// 自动连接 + 自动重连
void ServerConnection::doConnect()
{
    auto self = shared_from_this();
    m_socket.async_connect(
        tcp::endpoint(boost::asio::ip::make_address(m_ip), m_port),
        [this, self](const boost::system::error_code& ec) {
            if (!ec) {
                cout << "[连接成功] 服务器ID:" << m_serverId << " " << m_ip << ":" << m_port << endl;
                m_connected = true;
            } else {
                cout << "[连接失败] 重试... " << ec.message() << endl;
                m_connected = false;

                // 重连
                boost::asio::post(m_io, [this, self]() {
                    doConnect();
                });
            }
        });
}


//发送消息到后端服务（核心）
void ServerConnection::send(
    uint64_t playerId,
    uint64_t serverId,
    uint16_t protoId, 
    const char* data, 
    uint16_t len)
{
    if (!m_connected) {
        return;
    }

    // 组内部协议头
    InnerHead head;
    head.playerId = playerId;
    head.protoId  = protoId;
    head.ServerId = serverId;
    head.dataLen  = len;

    std::lock_guard<std::mutex> lock(m_sendMutex);

    // 发送：头 + 包体
    std::vector<boost::asio::const_buffer> bufs;
    bufs.push_back(boost::asio::buffer(&head, sizeof(head)));
    bufs.push_back(boost::asio::buffer(data, len));

    boost::asio::async_write(m_socket, bufs,
        [this, self = shared_from_this()](const boost::system::error_code& ec, size_t) {
            if (ec) {
                cerr << "[发送失败] 服务器ID:" << m_serverId << " 错误:" << ec.message() << endl;
                m_connected = false;
                m_socket.close();

                // 自动重连
                boost::asio::post(m_io, [this, self]() {
                    doConnect();
                });
            }
        });
}

// 发送数据
void ServerConnection::send(uint64_t playerId,uint16_t protoId, const char* data, uint16_t len)
{
    // 组内部协议头
    InnerHead head;
    head.playerId = playerId;
    head.protoId  = protoId;
    head.dataLen  = len;

    std::lock_guard<std::mutex> lock(m_sendMutex);

    // 发送：头 + 包体
    std::vector<boost::asio::const_buffer> bufs;
    bufs.push_back(boost::asio::buffer(&head, sizeof(head)));
    bufs.push_back(boost::asio::buffer(data, len));

    boost::asio::async_write(m_socket, bufs,
        [this, self = shared_from_this()](const boost::system::error_code& ec, size_t) {
            if (ec) {
                cerr << "[发送失败] 服务器ID:" << m_serverId << " 错误:" << ec.message() << endl;
                m_connected = false;
                m_socket.close();

                // 自动重连
                boost::asio::post(m_io, [this, self]() {
                    doConnect();
                });
            }
        });

}

// 发送数据
void ServerConnection::send(uint64_t playerId,uint16_t protoId)
{
    // 组内部协议头
    InnerHead head;
    head.playerId = playerId;
    head.protoId  = protoId;

    std::lock_guard<std::mutex> lock(m_sendMutex);

    // 发送：头 + 包体
    std::vector<boost::asio::const_buffer> bufs;
    bufs.push_back(boost::asio::buffer(&head, sizeof(head)));

    boost::asio::async_write(m_socket, bufs,
        [this, self = shared_from_this()](const boost::system::error_code& ec, size_t) {
            if (ec) {
                cerr << "[发送失败] 服务器ID:" << m_serverId << " 错误:" << ec.message() << endl;
                m_connected = false;
                m_socket.close();

                // 自动重连
                boost::asio::post(m_io, [this, self]() {
                    doConnect();
                });
            }
        });

}

void ServerConnection::send(LogicMessage* msg)
{
    // 组内部协议头
    InnerHead head;
    head.ServerId = msg->head.ServerId;
    head.protoId = msg->head.protoId;
    head.SceneId = msg->head.SceneId;
    head.playerId = msg->head.playerId;
    head.dataLen = msg->head.dataLen;

    std::lock_guard<std::mutex> lock(m_sendMutex);
    // 发送：头 + 包体
    std::vector<boost::asio::const_buffer> bufs;
    bufs.push_back(boost::asio::buffer(&head, sizeof(head)));
    bufs.push_back(boost::asio::buffer(msg->body, head.dataLen));

    boost::asio::async_write(m_socket, bufs,
        [this, self = shared_from_this()](const boost::system::error_code& ec, size_t) {
            if (ec) {
                cerr << "[发送失败] 服务器ID:" << m_serverId << " 错误:" << ec.message() << endl;
                m_connected = false;
                m_socket.close();

                // 自动重连
                boost::asio::post(m_io, [this, self]() {
                    doConnect();
                });
            }
        });

}