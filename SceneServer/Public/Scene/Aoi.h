// Aoi.h
#pragma once
#include <iostream>
#include <list>
#include <cstdint>

struct AoiPos {     //位置
    int x = 0;
    int y = 0;
};

class AoiManager {
public:
    static AoiManager& instance();
    //增加aoi对象
    static void addObject(uint64_t objid);
    //删除aoi对象
    static void removeObject(uint64_t objid);
    //更新
    //
    //aoi用九宫格 通过pos获取所在cell或者grid  然后获取周围的cell中的玩家的id
    std::list<int> AoiManager::GetNearBy(uint16_t id);
private:
    AoiManager() = default;
};