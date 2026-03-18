#pragma once
#include <stdint.h>

struct SkillConfig
{
    int skillId;
    int damage;          // 伤害值
    int castRange;       // 释放距离
    float cd;            // 冷却
    int aoeRange;        // 范围伤害
};


//技能配置管理者
class SkillConfigManager {
public:
    static SkillConfig* getSkillById(int id);
};