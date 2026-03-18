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
    //加入到aoi
    addToAOI(player->GetId(), player->GetPos());

}

//玩家离开
void Scene::playerLeave(uint64_t playerId) {
    auto it = _players.find(playerId);
    if (it != _players.end()) {
        it->second->onLeaveScene();
        removeFromAOI(playerId);
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


//aoi相关  通过pos算出grid下标
void Scene::posToGrid(AoiPos pos, int& gx, int& gy) {
    gx = (int)(pos.x / GRID_SIZE);
    gy = (int)(pos.y / GRID_SIZE);
}

void Scene::addToAOI(uint64_t oid, AoiPos pos) {
    _objPositions[oid] = pos;
    int gx, gy;
    posToGrid(pos, gx, gy);
    _grids[gx][gy].objects.insert(oid);
}

void Scene::addToAOI(uint64_t oid, AoiPos pos) {
    _objPositions[oid] = pos;
    int gx, gy;
    posToGrid(pos, gx, gy);
    _grids[gx][gy].objects.insert(oid);
}

void Scene::removeFromAOI(uint64_t oid) {
    auto it = _objPositions.find(oid);
    if (it == _objPositions.end()) return;

    int gx, gy;
    posToGrid(it->second, gx, gy);
    _grids[gx][gy].objects.erase(oid);
    _objPositions.erase(oid);
}

void Scene::updateAOI(uint64_t oid, AoiPos newPos) {
    removeFromAOI(oid);
    addToAOI(oid, newPos);
}

//获取周围对象 九宫格算法
std::unordered_set<uint64_t> Scene::getNearbyObjects(uint64_t oid) {
    std::unordered_set<uint64_t> res;
    auto it = _objPositions.find(oid);
    if (it == _objPositions.end()) return res;

    int gx, gy;
    posToGrid(it->second, gx, gy);

    for (int dx = -1; dx <= 1; dx++) {      //先对角线找 再横着
        for (int dy = -1; dy <= 1; dy++) {
            int nx = gx + dx;
            int nz = gy + dy;
            auto gridIter = _grids.find(nx);
            if (gridIter == _grids.end()) continue;
            auto subIter = gridIter->second.find(nz);
            if (subIter == gridIter->second.end()) continue;

            for (uint64_t id : subIter->second.objects) {
                if (id != oid) res.insert(id);
            }
        }
    }
    return res;
}
