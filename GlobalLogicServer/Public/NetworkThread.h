#pragma once
#include <thread>
#include <boost/asio.hpp>
#include "GlobalLogicDef.h"

using boost::asio::ip::tcp;

class NetworkThread {
public:
    static NetworkThread& instance();  //一个网络线程单例
    void start(uint16_t port);

private:
    NetworkThread();
    void run();
    void startAccept();
    void onAccept(boost::system::error_code ec);
    void readSession(std::shared_ptr<tcp::socket> sock);

    boost::asio::io_context _io;
    tcp::acceptor           _acceptor;
    std::thread             _thread;
};