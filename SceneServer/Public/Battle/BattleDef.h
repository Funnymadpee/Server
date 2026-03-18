#pragma once
#include <stdint.h>
#include <vector>

// 伤害来源
enum DamageOrigin {
    SELF = 1,
    OTHER = 2
};

// 战斗结果
struct BattleResult {
    uint64_t casterId;  //施法者
    uint64_t targetId;  //目标
    int skillId;        //技能id
    int damage;         //伤害
    bool isHit;         //是否击中
};