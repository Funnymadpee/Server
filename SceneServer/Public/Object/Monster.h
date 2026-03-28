#pragma once
#include "../Scene/Aoi.h"
#include "GameObject.h"
#include "AiBehaviorTree.h"
#include <memory>
#include <vector>

class Monster : public GameObject
{
public:
    Monster(int uid, float homeX, float homeY);

    // 重写 Object 的每帧更新
    void update(int delta) override;

    // 外部设置状态
    void setPos(float x, float y);
    void setHp(int hp);
    void setEnemy(bool hasEnemy, bool inAttackRange);

    // 获取
    float x() const { return _x; }
    float y() const { return _y; }
    int hp() const { return _hp; }

    // 获取视野内所有玩家
    std::vector<std::shared_ptr<GameObject>> getPlayersInView();
    

private:
    // AI 每帧执行
    void aiUpdate(int delta);
    void fillAiContext(AiContext& ctx);

private:
    int     _hp;
    int     _maxHp;
    float   _x, _y;
    float   _homeX, _homeY;
    bool    _hasEnemy;
    bool    _inAttackRange;

    

    BTNode::Ptr _aiTree;
};