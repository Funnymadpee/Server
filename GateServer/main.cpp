#include <iostream>
#include <thread>
#include <vector>
#include "./Public/GateServer.h"

using namespace std;

#define THREADSNUM 4
#define GateID 1            //暂时就这一个gate

int main() {

    // 解决 Windows 控制台中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        boost::asio::io_context ioc;
        GateServer server(ioc, GateID);
        server.start();         //创建启动gateserver

        vector<thread> threads;
        for (int i = 0; i < THREADSNUM; ++i) {
            threads.emplace_back([&] { ioc.run(); });   //asio线程池
        }

        for (auto& t : threads) t.join();
    }
    catch (const exception& e) {
        cerr << "有异常: " << e.what() << endl;
    }
    return 0;
}