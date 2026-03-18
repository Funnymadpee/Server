#include "../../Public/Battle/BattleSystem.h"
#include "../../Public/Scene/Scene.h"


void BattleSystem::useSkill(GameObject* caster, int skillId, uint64_t targetId)
{
    //取技能配置
    auto skill = SkillConfigManager::getSkillById(skillId);
    if (!skill) return;

    //找目标
    GameObject* target = caster->GetScene()->findObject(targetId);
    if (!target) return;

    //判断距离
    if (!BattleUtils::inRange(caster, target, skill->castRange))
        return;

    //计算伤害
    int damage = BattleUtils::calcDamage(caster, target, skill);

    //目标受击
    target->onHit(damage, caster->GetId());

    //广播给周围玩家
    broadcastSkill(caster, skillId, targetId, damage);
}


void BattleSystem::broadcastSkill(GameObject* caster, int skillId, uint64_t targetId, int damage)
{
    auto list = AoiManager::instance().GetNearBy(caster->GetId());
    for (uint64_t oid : list)
    {
         // 发送给客户端
    }
}

BattleSystem& BattleSystem::instance()
{
    static BattleSystem ins;
    return ins;
}