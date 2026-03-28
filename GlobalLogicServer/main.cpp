#include "./Public/GlobalLogicDef.h"
#include "./Public/NetworkThread.h"
#include "./Public/MessageQueue.h"
#include "./Public/LogicDispatcher.h"

int main() {
    // 解决 Windows 控制台中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const uint16_t PORT = 8889;
    const int MAX_PROCESS_MSG = 10;

    //启动网络线程
    NetworkThread::instance().start(PORT);

    //注册协议 连接nats  连接场景服
    LogicDispatcher::instance().init();

    //主线程逻辑线程
    while (true) {

        int processed = 0;
        //弹出消息进行处理
        //每帧固定处理消息的次数
        while (processed < MAX_PROCESS_MSG)
        {
            LogicMessage* msg = MessageQueue::instance().pop();
            if(msg == nullptr)
                break;
            LogicDispatcher::instance().dispatch(msg);
            delete msg;
            processed++;
        }
        //还要处理全局内部更新 *mgr->update()
        LogicDispatcher::instance().update();

    }

    return 0;
}