// SceneManager.h
#pragma once
#include <unordered_map>
#include <thread>
#include <vector>

#include "Scene.h"
#include "Aoi.h"

#define NUMOFSCENETHREAD 4

class SceneManager {
public:
    static SceneManager& instance();
    void init();
    void update(int delta); //更新场景内对象
    Scene* getScene(int sceneId);
    void broadcast_all();
private:
    std::unordered_map<int, Scene*> _scenes;
    SceneManager() = default;


    uint16_t _length;       //长度
    uint16_t _width;         //宽度
    uint16_t _sceneLength;   //格子长度
    uint16_t _sceneWidth;    //格子宽度

};