#ifndef GATE_ROUTER_H
#define GATE_ROUTER_H

#include <boost/asio.hpp>
#include "Router.h"

using boost::asio::ip::tcp;

class GateRouter {
public:
    GateRouter(boost::asio::io_context& ioc, int port, Router& router);

private:
    void doAccept();
    void startGateSession(tcp::socket socket);

private:
    tcp::acceptor acceptor_;
    Router& router_;
};

#endif