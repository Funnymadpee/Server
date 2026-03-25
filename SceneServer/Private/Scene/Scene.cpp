#include "../../Public/Scene/Scene.h"

Scene::Scene(int sceneId) : _sceneId(sceneId) {
}

Scene::~Scene() {
    //场景销毁 直接清理所有对象
    for (auto& pair : _objects) {
        delete pair.second;
    }
    _objects.clear();
}

//玩家进入
void Scene::playerEnter(Player* player) {
    _players[player->GetId()] = player;
    player->onEnterScene(_sceneId);


}

//玩家离开
void Scene::playerLeave(uint64_t playerId) {
    auto it = _players.find(playerId);
    if (it != _players.end()) {
        it->second->onLeaveScene();
        delete it->second;
        _players.erase(it);
    }

    auto itt = _objects.find(playerId);
    if (itt != _objects.end()) {
        delete itt->second;
        _objects.erase(itt);
    }
}


void Scene::update(int delta) {
    //如果场景内玩家为空 直接返回
    if(_players.empty()) return;

    //处理场景消息


    // 更新所有对象
    for (auto& [id, obj] : _objects) {
        obj->update(delta);
    }
}

void Scene::addObject(GameObject* obj)
{
}


void Scene::removeObject(uint64_t oid)
{

}

GameObject* Scene::findObject(uint64_t oid)
{
    auto it = _objects.find(oid);
    if(it!= _objects.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

//场景消息的处理
void Scene::push(SceneMessage* msg) {
    std::lock_guard<std::mutex> lock(_scenemutex);
    _sceneQueue.push(msg);
}

SceneMessage* Scene::pop() {
    std::unique_lock<std::mutex> lock(_scenemutex);
    SceneMessage* msg = _sceneQueue.front();
    _sceneQueue.pop();
    return msg;
}

void Scene::processMessage()
{
    auto msg = pop();
    if(msg!= nullptr)
    {
        ////根据协议id做处理
    }


}
