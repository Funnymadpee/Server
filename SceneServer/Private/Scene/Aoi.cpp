#include "../../Public/Scene/Aoi.h"
#include <cstdint>

std::list<int> AoiManager::GetNearBy(uint16_t id)
{
    std::list<int> ids{1,2,3};
    return ids;
}

AoiManager& AoiManager::instance()
{
    static AoiManager ins;
    return ins;
}