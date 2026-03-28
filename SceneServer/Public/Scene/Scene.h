#pragma once
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <mutex>

#include "../Object/GameObject.h"
#include "../Object/Player.h"
#include "Aoi.h"
#include "../SceneServerDef.h"

#define SCENESIZE 32

class GameObject;
class Player;

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


    //场景消息
    void push(SceneMessage* msg);
    SceneMessage* pop();
    void processMessage();  //场景消息处理

    int getLeftX(){return left_x;}
    int getRightX(){return right_x;}
    int getTopY(){return top_y;}
    int getBottonY(){return botton_y;}

private:
    int _sceneId;       //场景id
    std::unordered_map<uint64_t, GameObject*> _objects;   //场景内对象
    std::unordered_map<uint64_t, Player*> _players;         //场景内玩家

    //区域内的消息队列
    std::queue<SceneMessage*>  _sceneQueue;
    std::mutex                  _scenemutex;


    //场景边界
    int left_x, right_x;
    int top_y, botton_y;


};