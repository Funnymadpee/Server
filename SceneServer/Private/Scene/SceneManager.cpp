#include "../../Public/Scene/SceneManager.h"

void SceneManager::update(int delta) {
    for (auto& [id, scene] : _scenes) {
        scene->update(delta);
    }
}

void SceneManager::init()
{

}

Scene* SceneManager::getScene(int sceneId)
{
    auto it = _scenes.find(sceneId);
    if(it!= _scenes.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

SceneManager& SceneManager::instance()
{
        static SceneManager ins;
        return ins;
}