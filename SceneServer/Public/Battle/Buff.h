#pragma once
#include <unordered_map>
#include <cstdint>

struct Buff {
    int id;         //buff id

    //对于DOT型技能
    int duration = 5000;    // 总持续时间
    int elapsed = 0;        // 已过时间
    int triggerTimer = 0;   // 距离下次触发的时间
    int interval = 0;           //间隔
};

class BuffManager {
public:
    BuffManager(){}
    void add(int id);

    void update(int deltaMs);

    std::unordered_map<int, Buff> _buffs;  // manageer是绑定在玩家对象上的
};