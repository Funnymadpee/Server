#include "../../Public/Battle/Buff.h"


void BuffManager::add(int id) {
    _buffs[id] = {id};
}

void BuffManager::update(int deltaMs) {
    for (auto buff = _buffs.begin(); buff != _buffs.end();) {
        //更改buff

        //对对于dot
        if (buff->second.triggerTimer >= buff->second.interval)
            {
                //触发buff
                buff->second.triggerTimer = 0;
            }

            // 超时移除
            if (buff->second.elapsed >= buff->second.duration)
                buff = _buffs.erase(buff);
            else
                ++buff;
    }
}