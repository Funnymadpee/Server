#include "../../Public/Object/GameObject.h"
#include "../../Public/Battle/BattleDef.h"

// 每帧更新
void GameObject::update(int delta) {
    buffMgr->update(delta);
}

// 受击
void GameObject::onHit(int damage, uint64_t casterId) {
    hp = hp < damage ? 0 : hp - damage;
    if (hp <= 0) onDie(casterId);
}

// 死亡
void GameObject::onDie(uint64_t killerId)
{}
