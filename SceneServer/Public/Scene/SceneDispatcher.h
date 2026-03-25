// SceneDispatcher.h
#pragma once
#include <unordered_map>
#include <functional>
#include "../SceneServerDef.h"
#include "SceneManager.h"
#include "../MessageQueue.h"

class SceneDispatcher {
public:
    static SceneDispatcher& instance();
    void init();

    //根据区域id进行消息分发到具体区域
    void dispatch(SceneMessage* msg);
private:
    SceneDispatcher() = default;
};