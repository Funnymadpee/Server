#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <mutex>
#include <atomic>

using boost::asio::ip::tcp;

// router的连接代理 连到后端服务
class ServerConnection : public std::enable_shared_from_this<ServerConnection>
{
public:
    using Pointer = std::shared_ptr<ServerConnection>;

    static Pointer create(boost::asio::io_context& io, const std::string& ip, int port, int serverId) {
        return std::make_shared<ServerConnection>(io, ip, port, serverId);
    }

    ServerConnection(boost::asio::io_context& io, const std::string& ip, int port, int serverId);
    ~ServerConnection();

    // 启动连接
    void start();

    // 发送数据
    void send(uint64_t playerId, uint64_t serverId,
              uint16_t protoId, const char* data, uint16_t len);

    // 连接状态
    bool isConnected() const { return m_connected; }
    int serverId() const { return m_serverId; }

private:
    void doConnect();
    void doSend(const char* data, size_t len);
    void onSendComplete(const boost::system::error_code& ec, size_t);

private:
    boost::asio::io_context&  m_io;
    tcp::socket               m_socket;
    std::string               m_ip;
    int                       m_port;
    int                       m_serverId;   //集群服id
    std::atomic<bool>         m_connected{false};
    std::mutex                m_sendMutex;
};