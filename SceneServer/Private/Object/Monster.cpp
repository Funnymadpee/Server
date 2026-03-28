#include "../../Public/Object/Monster.h"

Monster::Monster(int uid, float homeX, float homeY)
    : GameObject(uid, homeX, homeY)
    , _hp(100)
    , _maxHp(100)
    , _x(homeX)
    , _y(homeY)
    , _homeX(homeX)
    , _homeY(homeY)
    , _hasEnemy(false)
    , _inAttackRange(false)
{
    _aiTree = CreateMonsterAI();
}

void Monster::update(int delta)
{
    // 每帧执行AI
    aiUpdate(delta);
}

void Monster::aiUpdate(int delta)
{
    if (!_aiTree) return;

    AiContext ctx;
    fillAiContext(ctx);

    _aiTree->tick(ctx);

    // 同步位置回来
    _x = ctx.x;
    _y = ctx.y;
}

void Monster::fillAiContext(AiContext& ctx)
{
    ctx.uid = id;
    ctx.hp = _hp;
    ctx.max_hp = _maxHp;
    ctx.x = _x;
    ctx.y = _y;
    ctx.home_x = _homeX;
    ctx.home_y = _homeY;
    ctx.has_enemy = _hasEnemy;
    ctx.in_attack_range = _inAttackRange;

    //填充aoi玩家
    ctx.players_in_view = getPlayersInView();
}

void Monster::setPos(float x, float y)
{
    _x = x;
    _y = y;
}

void Monster::setHp(int hp)
{
    _hp = hp < 0 ? 0 : hp;
}

void Monster::setEnemy(bool hasEnemy, bool inAttackRange)
{
    _hasEnemy = hasEnemy;
    _inAttackRange = inAttackRange;
}



// AOI：获取视野内所有玩家（多个）
std::vector<std::shared_ptr<GameObject>> Monster::getPlayersInView()
{
    std::vector<std::shared_ptr<GameObject>> players;


    auto objs = AoiManager::instance().GetNearBy(id);

    for (auto& obj : objs)
    {
        //只有玩家类才放进去
    }
    return players;
}
