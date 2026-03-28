#include "../../Public/Scene/SceneDispatcher.h"
#include <cstring>

void SceneDispatcher::init(){
   
}

void SceneDispatcher::dispatch(SceneMessage* msg)
{
    //先通过消息获取到sceneid 压入具体的场景区域的消息队列
    int index = msg->head.SceneId;
    auto scene = SceneManager::instance().getScene(index);
    if(scene!=nullptr)
    {
        auto newMsg = new SceneMessage();
        newMsg->head = msg->head;
        memcpy(newMsg->body, msg->body, sizeof(newMsg->body));
        scene->push(newMsg);
    }
    

}

SceneDispatcher& SceneDispatcher::instance()
{
    static SceneDispatcher ins;
    return ins;
}