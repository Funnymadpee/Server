#include "../../Public/Scene/SceneDispatcher.h"

void SceneDispatcher::init(){
   
}

void SceneDispatcher::dispatch(InnerHead* head, const char* body)
{

}

void SceneDispatcher::handleEnterScene(InnerHead* head, const char* body)
{

}

void SceneDispatcher::handleMove(InnerHead* head, const char* body)
{

}

void SceneDispatcher::handleUseSkill(InnerHead* head, const char* body)
{

}

SceneDispatcher& SceneDispatcher::instance()
{
    static SceneDispatcher ins;
    return ins;
}