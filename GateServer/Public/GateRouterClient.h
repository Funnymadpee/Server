#ifndef GATE_ROUTER_CLIENT_H
#define GATE_ROUTER_CLIENT_H

#include <boost/asio.hpp>
#include "../../Common/G2RInnerProtocol.h"

using boost::asio::ip::tcp;

class GateRouterClient: public std::enable_shared_from_this<GateRouterClient> {
public:
    GateRouterClient(boost::asio::io_context& ioc);
    void connect(const char* ip, int port);
    void send(const char* data, int len);
    void send(const char* headData, size_t headLen, 
                            const char* bodyData, size_t bodyLen);


private:
    tcp::socket sock;
    bool connected = false;
};

#endif