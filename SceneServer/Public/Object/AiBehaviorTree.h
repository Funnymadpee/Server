#pragma once
#include "GameObject.h"
#include <memory>
#include <vector>

//ai行为树节点执行状态
enum class BTStatus
{
    Success,
    Failure,
    Running
};

///ai上下文
struct AiContext
{
    int     uid             = 0;
    int     hp              = 100;
    int     max_hp          = 100;
    bool    has_enemy       = false;
    bool    in_attack_range = false;
    float   x               = 0.0f;
    float   y               = 0.0f;
    float   home_x          = 0.0f;
    float   home_y          = 0.0f;

    // 视野内所有玩家
    std::vector<std::shared_ptr<GameObject>> players_in_view;
};

///节点基类
class BTNode
{
public:
    using Ptr = std::shared_ptr<BTNode>;
    virtual ~BTNode() = default;
    virtual BTStatus tick(AiContext& ctx) = 0;
};

// 记忆选择节点
class MemSelector : public BTNode
{
public:
    void addChild(BTNode::Ptr child);   //子节点
    BTStatus tick(AiContext& ctx) override; //
private:
    std::vector<BTNode::Ptr> _children;
    size_t _last_running_idx = 0;       //记录上次执行节点位置
};

// 记忆顺序节点
class MemSequence : public BTNode
{
public:
    void addChild(BTNode::Ptr child);
    BTStatus tick(AiContext& ctx) override;
private:
    std::vector<BTNode::Ptr> _children;
    size_t _last_running_idx = 0;
};

// 条件节点
class ConditionHpLow : public BTNode { BTStatus tick(AiContext& ctx) override; };
class ConditionHasEnemy : public BTNode { BTStatus tick(AiContext& ctx) override; };
class ConditionInAttackRange : public BTNode { BTStatus tick(AiContext& ctx) override; };

// 行为节点
class ActionFleeHome : public BTNode { BTStatus tick(AiContext& ctx) override; };
class ActionMoveToTarget : public BTNode { BTStatus tick(AiContext& ctx) override; };
class ActionAttack : public BTNode { BTStatus tick(AiContext& ctx) override; };
class ActionIdle : public BTNode { BTStatus tick(AiContext& ctx) override; };

// 创建AI树
BTNode::Ptr CreateMonsterAI();