// Aoi.h
#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cstdint>

enum AoiNodeState
{
    ADD = 0,
    UPDATE,
    DELETE
};

struct AoiNode {     //aoi节点
    int id; //玩家id
    float x;
    float y;
    float aoi_radius;   // AOI视野半径
    AoiNodeState state; //状态

    //更新用
    float newX;
    float newY;

    AoiNode(int id_, float x_, float y_, float radius_ = 50.0f, AoiNodeState state_ = AoiNodeState::ADD)
        : id(id_), x(x_), y(y_), aoi_radius(radius_),state(state_) {}
};


//////四叉树  以左上角为原点
class QuadTree {
public:
    static const int MAX_DEPTH = 6;       // 最大递归深度 4^6*8
    static const int MAX_NODES = 8;    // 单节点最大节点数

    // 节点边界
    float left, right;
    float top, bottom;
    int depth;  //深度

    //起点
    float oriX;
    float oriY;

    // 子树
    QuadTree* children[4];
    // 存放aoinode节点
    std::vector<AoiNode*> nodes;

public:
    // 构造函数： 根节点坐标 + 宽高
    QuadTree(float ori_x, float orir_y, float width, float height, int depth_ = 0)
        : depth(depth_) {
        // 计算边界
        left = ori_x;
        right = ori_x + width / 2;
        top = orir_y;
        bottom = orir_y + height / 2;

        oriX = ori_x;
        oriY = orir_y;

        // 初始化子节点为空  顺时针 左上 右上
        children[0] = children[1] = children[2] = children[3] = nullptr;
    }

    // 析构函数：释放子节点
    ~QuadTree() {
        clear();
    }

    // 清空四叉树
    void clear();

    // 判断是否在当前节点内
    bool contains(AoiNode* node) const;

    // 切分为4个子节点
    void split();

    // 插入到四叉树
    bool insert(AoiNode* node);

    // 删除
    bool remove(int id);

    // AOI查询
    void query(AoiNode* target, std::vector<AoiNode*>& result);

    //根据位置查询节点
    QuadTree* findTreeByPos(QuadTree* node, float x, float y);

};



class AoiManager {
public:
    static AoiManager& instance();
    //初始化
    static void init();

    //销毁
    static void destroy();
    static void clearAll();

    //增加node
    static void addNode(AoiNode* node);

    //更新
    static void update(int delat); 

    //删除node
    static void removeNode(AoiNode* node);

    std::list<int> GetNearBy(uint16_t id);

private:
    AoiManager() = default;


public:
    static QuadTree* root;                      // 四叉树根节点
    static std::unordered_map<int, AoiNode*> node_map;  // 节点字典
    //scene自己存一个脏标记数组 所有场景线程更新完上报给aoiManager
    static std::vector<AoiNode*>    modifys;    //有变更标记的 给update
};