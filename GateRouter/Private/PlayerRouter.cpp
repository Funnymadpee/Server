#include "PlayerRouter.h"

PlayerRouter& PlayerRouter::instance()
{
    static PlayerRouter ins;
    return ins;
}

// 绑定玩家到服务器
void PlayerRouter::bindPlayer(uint64_t playerId, ServerType type, int serverId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_playerMap[playerId] = { type, serverId };
}

// 解绑
void PlayerRouter::unbindPlayer(uint64_t playerId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_playerMap.erase(playerId);
}

// 查询玩家在哪
bool PlayerRouter::getPlayerServer(uint64_t playerId, ServerType& outType, int& outId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_playerMap.find(playerId);
    if (it == m_playerMap.end()) {
        return false;
    }
    outType = it->second.serverType;
    outId = it->second.serverId;
    return true;
}