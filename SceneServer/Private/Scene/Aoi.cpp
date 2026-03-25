#include "../../Public/Scene/Aoi.h"

class QuadTree;


void QuadTree::clear()
 {
    for (int i = 0; i < 4; ++i) {
        if (children[i] != nullptr) {
            children[i]->clear();
        }
        else
        {
            nodes.clear();
        }
    }
}

bool QuadTree::contains(AoiNode* node) const {
        return (node->x >= left && node->x <= right) &&
               (node->y >= top && node->y <= bottom);
    }


void QuadTree::split() 
{
    float w = (right - left) / 2;
    float h = (bottom - top) / 2;
    float ox = left;
    float oy = top;

    // 创建四个子节点
    children[0] = new QuadTree(ox, oy, w, h, depth + 1); // 坐上
    children[1] = new QuadTree(ox + w/2, oy, w, h, depth + 1); // 右上
    children[2] = new QuadTree(ox, oy + h/2, w, h, depth + 1); // 坐下
    children[3] = new QuadTree(ox + w/2, oy + h/2, w, h, depth + 1); // 右下
}


bool QuadTree::insert(AoiNode* node) 
{
    //边界检查
    if (!contains(node)) return false;

    // 未达到容量上限 或 达到最大深度，直接存储
    if (nodes.size() < MAX_NODES || depth >= MAX_DEPTH) {
        nodes.push_back(node);
        return true;
    }

    // 容量满了，分裂节点
    if(children[0] == nullptr)
    {
        //确保只分裂一次
        split();
    }

    // 插入到对应子节点
    for (int i = 0; i < 4; ++i) {
         if (children[i]->insert(node)) {
            return true;
        }
    }
    return false;
}


bool QuadTree::remove(int id)
{
// 先在当前节点找
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if ((*it)->id == id) {
                nodes.erase(it);
                return true;
            }
        }
        // 没子节点 → 找不到
        if (children[0] == nullptr) return false;
        // 递归去子节点删
        for(int i = 0; i < 4; i++)
        {
            children[i]->remove(id);
        }
        return false;
}


//aoi查询
void QuadTree::query(AoiNode* target, std::vector<AoiNode*>& result) 
{
    if (!contains(target)) return;

    // 添加当前节点的所有实体
    for (auto* e : nodes) {
        if (e->id != target->id) {
            result.push_back(e);
        }
    }

    // 没有子节点，结束递归
    if (children[0] == nullptr) return;

    // 递归查询子节点
    for (int i = 0; i < 4; ++i) {
        children[i]->query(target, result);
    }
}





///////////////////////////////////////////////////
std::unordered_map<int, AoiNode*> AoiManager::node_map;
QuadTree* AoiManager::root = nullptr;
std::vector<AoiNode*> AoiManager::modifys;

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

void AoiManager::init()
{
    //参数暂时写死  左上角为原点
    float map_width = 512.0f;
    float map_height = 512.0f;
    root = new QuadTree(0, 0, map_width, map_height);
}

void AoiManager::destroy()
{
    clearAll();
    delete root;
}

void AoiManager::clearAll() 
{
    for (auto& pair : node_map) {
        delete pair.second;
    }
    node_map.clear();
    root->clear();
}

void AoiManager::addNode(AoiNode* node)
{
    if (!node) return;
    node_map[node->id] = node;
    root->insert(node);
}



void AoiManager::update(int delat)
{
    for(auto node: modifys)
    {
        switch (node->state)
        {
        case AoiNodeState::ADD:
            addNode(node);
            break;

        case AoiNodeState::UPDATE:
            //先删再插
            removeNode(node);
            node->x = node->newX;
            node->y = node->newY;
            addNode(node);
            break;

        case AoiNodeState::DELETE:
            removeNode(node);
        
        default:
            break;
        }
    }
}

void AoiManager::removeNode(AoiNode* node)
{
    if (!node) return;
    node_map.erase(node->id);
    root->remove(node->id);
}