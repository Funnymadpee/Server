#include "../../Public/Scene/SceneDispatcher.h"

void SceneDispatcher::init(){
   
}

void SceneDispatcher::dispatch(SceneMessage* msg)
{
    //先通过消息获取到sceneid 压入具体的场景区域的消息队列
    int index = msg->head.SceneId;
    auto scene = SceneManager::instance().getScene(index);
    scene->push(msg);

}

SceneDispatcher& SceneDispatcher::instance()
{
    static SceneDispatcher ins;
    return ins;
}