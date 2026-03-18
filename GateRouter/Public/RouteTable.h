#pragma once

#include <unordered_map>
#include <stdint.h>
#include <mutex>
#include <string>

enum class ServerType : uint8_t
{
    Unknown        = 0,
    LoginServer    = 1,     //登录服
    LogicServer    = 2,     //逻辑服
    SceneServer    = 3,     //场景服
    ChatServer     = 4,     
    GuildServer    = 5,
    RankServer     = 6,
};


//业务服务路由表   协议  绑定 服务器类型
class RouteTable
{
public:
    static RouteTable& instance();

    // 初始化配置
    bool init(const std::string& configPath);

    // 热更新配置
    bool reload();

    // 获取目标服
    ServerType getTargetServer(uint16_t protoId) const;

private:
    RouteTable() = default;

    // 从文件加载
    bool loadFromFile(const std::string& path);

private:
    std::unordered_map<uint16_t, ServerType> m_routes;          //协议号 服务器类型  后面再根据服务器类型到集群中的某台
    mutable std::mutex m_mutex;
    std::string m_configPath;
};