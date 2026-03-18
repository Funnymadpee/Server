#include "../../Public/Battle/BattleUtils.h"


// 计算最终伤害
int BattleUtils::calcDamage(GameObject* caster, GameObject* target, SkillConfig* skill)
{
    int dmg = skill->damage;
    // 这里可以加 攻击-防御、暴击、命中、抗性...
    return dmg;
}

// 是否在射程内
bool BattleUtils::inRange(GameObject* a, GameObject* b, int range)
{
    //距离判断  勾股定理
    int dx = a->GetX() - b->GetX();
    int dy = a->GetY() - b->GetY();
    return dx * dx + dy * dy <= range * range;
}