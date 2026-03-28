#include "../../Public/Scene/SceneManager.h"
#include "../../Common/ThreadPool.h"
#include <thread>
#include <vector>

void SceneManager::update(int delta) {
    auto& pool = ThreadPool::instance();
    //将场景区域的更新封装成任务投递给线程池
        for (auto& scene : _scenes) {
            pool.enqueue([&scene, delta]() {
                scene.second->update(delta);
            });
        }

        //主线程统一广播消息
        broadcast_all();

}

void SceneManager::broadcast_all() {
        // std::cout << "主线程：所有区域更新完毕，开始广播消息\n";
    }

void SceneManager::init()
{
    std::cout << "场景开始初始化\n";
    //读取场景参数  暂时写死
    _length = 256;       //长度
    _width = 256;         //宽度
    _sceneLength = 32;   //格子长度
    _sceneWidth = 32;    //格子宽度   这样一张图就有16*16 = 64 个scene

    //左上角为原点0,0  从左到右 从上到下
    int index = 0;
    for(int j = 0; j< 256; j+=32)
    {
        for(int i = 0; i < 256; i+=32)
        {
            Scene* scene = new Scene(index);
            _scenes.emplace(std::pair<int, Scene*>(index, scene));
            index++;
        }
    }
    
    

}

Scene* SceneManager::getScene(int sceneId)
{
    auto it = _scenes.find(sceneId);
    if(it!= _scenes.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

SceneManager& SceneManager::instance()
{
        static SceneManager ins;
        return ins;
}