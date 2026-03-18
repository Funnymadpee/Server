#pragma once
#include "Skill.h"
#include "BattleUtils.h"
#include "../Scene/Aoi.h"

class GameObject;

class BattleSystem
{
public:

    static void useSkill(GameObject* caster, int skillId, uint64_t targetId);

    static void broadcastSkill(GameObject* caster, int skillId, uint64_t targetId, int damage);
   
    static BattleSystem& instance();
private:
    BattleSystem() = default;
};