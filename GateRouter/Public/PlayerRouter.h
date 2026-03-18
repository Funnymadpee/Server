#pragma once
#include <stdint.h>
#include <unordered_map>
#include "RouteTable.h"

// 玩家所在服务器信息
struct PlayerServer
{
    ServerType  serverType;     // Login/Logic/Scene/Chat
    int         serverId;       // 服务器编号（A/B/C服）
};

// 全局玩家场景服路由表
class PlayerRouter
{
public:
    static PlayerRouter& instance();

    // 绑定：玩家 → 某台服务器
    void bindPlayer(uint64_t playerId, ServerType type, int serverId);

    // 解绑：玩家切换服务器/下线
    void unbindPlayer(uint64_t playerId);

    // 获取玩家所在服务器
    bool getPlayerServer(uint64_t playerId, ServerType& outType, int& outId);

private:
    PlayerRouter() = default;   //单例

private:
    std::unordered_map<uint64_t, PlayerServer> m_playerMap;     //玩家id  服务器信息
    mutable std::mutex m_mutex;
};