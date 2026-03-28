#include "../../Public/Object/AiBehaviorTree.h"

void MemSelector::addChild(BTNode::Ptr child) { _children.push_back(child); }
BTStatus MemSelector::tick(AiContext& ctx)      //选择节点每帧更新
{
    for (size_t i = _last_running_idx; i < _children.size(); ++i)
    {
        auto s = _children[i]->tick(ctx);
        if (s == BTStatus::Running) { _last_running_idx = i; return BTStatus::Running; }        //记录节点 位置
        if (s == BTStatus::Success) { _last_running_idx = 0; return BTStatus::Success; }
    }
    _last_running_idx = 0;  //
    return BTStatus::Failure;
}

void MemSequence::addChild(BTNode::Ptr child) { _children.push_back(child); }
BTStatus MemSequence::tick(AiContext& ctx)  //顺序节点每帧更新
{
    for (size_t i = _last_running_idx; i < _children.size(); ++i)
    {
        auto s = _children[i]->tick(ctx);
        if (s == BTStatus::Running) { _last_running_idx = i; return BTStatus::Running; }
        if (s == BTStatus::Failure) { _last_running_idx = 0; return BTStatus::Failure; }
    }
    _last_running_idx = 0;
    return BTStatus::Success;
}

BTStatus ConditionHpLow::tick(AiContext& ctx)       //判断血量节点
{
    return (ctx.hp * 100 / ctx.max_hp < 30) ? BTStatus::Success : BTStatus::Failure;
}

BTStatus ConditionHasEnemy::tick(AiContext& ctx)            //是否有敌人
{
    return ctx.has_enemy ? BTStatus::Success : BTStatus::Failure;
}

BTStatus ConditionInAttackRange::tick(AiContext& ctx)
{
    return ctx.in_attack_range ? BTStatus::Success : BTStatus::Failure;
}

BTStatus ActionFleeHome::tick(AiContext& ctx)
{
    float dx = ctx.x - ctx.home_x;
    float dy = ctx.y - ctx.home_y;
    if (dx*dx + dy*dy > 0.5f)
    {
        ctx.x += (ctx.home_x - ctx.x) * 0.1f;
        ctx.y += (ctx.home_y - ctx.y) * 0.1f;
        return BTStatus::Running;
    }
    return BTStatus::Success;
}

BTStatus ActionMoveToTarget::tick(AiContext& ctx)
{
    if (!ctx.has_enemy) return BTStatus::Failure;
    if (ctx.in_attack_range) return BTStatus::Success;
    ctx.x += 0.1f;
    return BTStatus::Running;
}

BTStatus ActionAttack::tick(AiContext& ctx)
{
    // 攻击逻辑
    return BTStatus::Success;
}

BTStatus ActionIdle::tick(AiContext& ctx)
{
    return BTStatus::Success;
}

BTNode::Ptr CreateMonsterAI()
{
    //创捷行为树 选择
    auto root = std::make_shared<MemSelector>();

    //巡逻分支
    auto flee_seq = std::make_shared<MemSequence>();
    flee_seq->addChild(std::make_shared<ConditionHpLow>());
    flee_seq->addChild(std::make_shared<ActionFleeHome>());

    //战斗分支
    auto fight_seq = std::make_shared<MemSequence>();
    fight_seq->addChild(std::make_shared<ConditionHasEnemy>());
    fight_seq->addChild(std::make_shared<ActionMoveToTarget>());
    fight_seq->addChild(std::make_shared<ConditionInAttackRange>());
    fight_seq->addChild(std::make_shared<ActionAttack>());

    root->addChild(flee_seq);
    root->addChild(fight_seq);
    //空闲分支
    root->addChild(std::make_shared<ActionIdle>());

    return root;
}