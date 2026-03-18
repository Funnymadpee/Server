#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;

int main() {
    io_context ioc;
    cout << "Boost 配置成功！" << endl;
    return 0;
}