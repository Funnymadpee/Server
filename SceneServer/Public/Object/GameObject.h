#pragma once
#include "../Battle/BattleDef.h"
#include "../Battle/Buff.h"
#include "../Scene/Aoi.h"

class Scene;

class GameObject {
public:
    GameObject(uint64_t id, float mx, float my) 
        : id(id), buffMgr(new BuffManager()), pos(id, x ,y) {
            x = mx;
            y = my;
    }
    virtual ~GameObject() = default;

    // 每帧更新
    virtual void update(int delta);

    // 受击
    virtual void onHit(int damage, uint64_t casterId);

    // 死亡
    virtual void onDie(uint64_t killerId) = 0;

    // 位置
    int GetX(){return x;}
    int GetY(){return y;}
    AoiNode GetAoiNode(){ return pos;}
    Scene* GetScene(){return Scene!=nullptr?Scene:nullptr;}

    uint64_t GetId(){return id;}
    

    // 属性
    uint64_t id;
    AoiNode pos;
    float x, y;
    int hp = 100;
    int maxHp = 100;
    BuffManager* buffMgr;
    int sceneId;
    Scene*  Scene;  //所在的场景区域
};
