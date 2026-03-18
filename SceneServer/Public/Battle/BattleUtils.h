#pragma once
#include "../Object/GameObject.h"
#include "Skill.h"

class BattleUtils
{
public:
    // 计算最终伤害
    static int calcDamage(GameObject* caster, GameObject* target, SkillConfig* skill);

    // 是否在射程内
    static bool inRange(GameObject* a, GameObject* b, int range);
};