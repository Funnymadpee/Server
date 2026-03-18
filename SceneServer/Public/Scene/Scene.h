#pragma once
#include <unordered_map>
#include <unordered_set>
#include "../Object/GameObject.h"
#include "../Object/Player.h"
#include "Aoi.h"

#define GRID_SIZE 10

class GameObject;
class Player;

class Grid{
public:
    Grid();
    //格子里的对象
    std::unordered_set<uint64_t> objects;
};

class Scene {
public:
    Scene(int sceneId);
    ~Scene();

    void update(int delta); // 场景每帧更新

    //对象
    void addObject(GameObject* obj);
    void removeObject(uint64_t oid);
    GameObject* findObject(uint64_t oid);

    //玩家
    void playerEnter(Player* player);
    void playerLeave(uint64_t playerId);
    Player* findPlayer(uint64_t playerId);

    //aoi
    void addToAOI(uint64_t oid, AoiPos pos);
    void removeFromAOI(uint64_t oid);
    void updateAOI(uint64_t oid, AoiPos newPos);
    std::unordered_set<uint64_t> getNearbyObjects(uint64_t oid);

private:
    // 计算格子所在的坐标
    void posToGrid(AoiPos pos, int& gx, int& gy);

private:
    int _sceneId;       //场景id
    std::unordered_map<uint64_t, GameObject*> _objects;
    std::unordered_map<uint64_t, Player*> _players;
    //地图格子  坐标序号-->grid
    std::unordered_map<int, std::unordered_map<int, Grid>> _grids; 
    //对象所在位置
    std::unordered_map<uint64_t, AoiPos> _objPositions;
};