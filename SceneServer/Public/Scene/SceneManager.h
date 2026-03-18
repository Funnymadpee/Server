// SceneManager.h
#pragma once
#include <unordered_map>
#include "Scene.h"

class SceneManager {
public:
    static SceneManager& instance();
    void init();
    void update(int delta); //更新场景内对象
    Scene* getScene(int sceneId);
private:
    std::unordered_map<int, Scene*> _scenes;
    SceneManager() = default;
};