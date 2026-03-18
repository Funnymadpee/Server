// SceneDispatcher.h
#pragma once
#include <unordered_map>
#include <functional>
#include "../SceneServerDef.h"

class SceneDispatcher {
public:
    static SceneDispatcher& instance();
    void init();
    void dispatch(InnerHead* head, const char* body);
private:
    void handleEnterScene(InnerHead* head, const char* body); // 进入场景
    void handleMove(InnerHead* head, const char* body);       // 移动
    void handleUseSkill(InnerHead* head, const char* body);   // 技能

    using Handler = std::function<void(InnerHead*, const char*)>;
    std::unordered_map<uint16_t, Handler> _handlers;

    SceneDispatcher() = default;
};