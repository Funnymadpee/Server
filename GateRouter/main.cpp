#include <iostream>
#include <thread>
#include <chrono>    // for std::chrono::milliseconds
#include <vector>
#include "./Public/Router.h"
#include "./Public/GateRouter.h"
#include "./Public/RouteTable.h"

using namespace std;

#define ROUTER_PORT 9999
#define THREADS 4

int main() {
    // 解决 Windows 控制台中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    try {
        boost::asio::io_context ioc;
        Router router;
        GateRouter server(ioc, ROUTER_PORT, router);
        //路由表加载
        RouteTable::instance().init("route.txt");

        cout << "Router 启动 端口:" << ROUTER_PORT << endl;

        // 网络线程
        thread net_thread([&]() {
            ioc.run();
        });

        // 多逻辑线程
        const int LOGIC_THREAD_COUNT = 3; // 开3个逻辑线程
        vector<thread> logic_threads;

        for (int i = 0; i < LOGIC_THREAD_COUNT; ++i) {
            logic_threads.emplace_back([&router, i]() {
                while (true) {
                    // 从队列消费并处理消息
                    router.update();
                    // 线程休眠，降低CPU占用
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            });
        }

        // 等待线程
        net_thread.join();
        for (auto& t : logic_threads) t.join();
    }
    catch (exception& e) {
        cerr << "异常:" << e.what() << endl;
    }
    return 0;
}