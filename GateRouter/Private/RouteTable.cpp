#include "RouteTable.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//单例
RouteTable& RouteTable::instance()
{
    static RouteTable ins;
    return ins;
}

bool RouteTable::init(const string& configPath)
{
    m_configPath = configPath;
    return loadFromFile(configPath);
}

bool RouteTable::reload()
{
    cout << "[RouteTable] 开始热更路由表..." << endl;
    return loadFromFile(m_configPath);
}


// 从配置文件加载路由
bool RouteTable::loadFromFile(const string& path)
{
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "[RouteTable] 加载失败：" << path << endl;
        return false;
    }

    unordered_map<uint16_t, ServerType> tempRoutes;

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        uint16_t protoId;
        int serverType;

        // 格式：protoId  serverType
        ss >> protoId >> serverType;

        tempRoutes[protoId] = (ServerType)serverType;
    }

    // 加锁替换
    lock_guard<mutex> lock(m_mutex);
    m_routes.swap(tempRoutes);

    cout << "[RouteTable] 热更完成！共加载 " << m_routes.size() << " 条路由" << endl;
    return true;
}


// 获取路由
ServerType RouteTable::getTargetServer(uint16_t protoId) const
{
    lock_guard<mutex> lock(m_mutex);
    auto it = m_routes.find(protoId);
    if (it != m_routes.end()) {
        return it->second;
    }
    return ServerType::Unknown;
}