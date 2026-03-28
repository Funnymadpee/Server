#include "../../Public/Scene/Scene.h"

Scene::Scene(int sceneId) : _sceneId(sceneId) {
    //计算边界
    int div = sceneId / 16;   //行列各16个
    int mod = sceneId % 16;


    left_x = mod * 32;
    right_x = (mod + 1 ) * 32;

    top_y = div * 32;
    botton_y = (div + 1) * 32;

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

    if(_sceneQueue.empty())
        return;

    //处理场景消息
    processMessage();


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
    std::lock_guard<std::mutex> lock(_scenemutex);
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
        if(msg->head.protoId == 7001)
        {
            //角色进入场景  没有就创建角色
            LocData loc_data;
            memcpy(&loc_data, msg->body, sizeof(LocData));

            int playerid = msg->head.playerId;
            auto it = _players.find(playerid);
            if(it == _players.end())
            {
                auto newpalyer = new Player(playerid, loc_data.loc_x, loc_data.loc_y);
                playerEnter(newpalyer);
                
            }
            else
            {
                //调用进入场景
                auto player = it->second;
                playerEnter(player);
            }
        }

        delete msg;
    }


}
