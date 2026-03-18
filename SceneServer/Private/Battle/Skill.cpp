#include "../../Public/Battle/Skill.h"


//
SkillConfig* SkillConfigManager::getSkillById(int id) 
{
    static SkillConfig cfg;
    return &cfg;
}