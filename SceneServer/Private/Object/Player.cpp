#include "../../Public/Object/Player.h"
#include "../../Public/Scene/Aoi.h"
#include "../../Public/Scene/SceneManager.h"
#include <cstdio>
#include <chrono>
#include <iostream>

class SceneManager;

Player::Player(uint64_t playerId, float mx, float my)
    : GameObject(playerId,mx, my)
    , _playerId(playerId)
    , _sceneId(0)
    , _state(PLAYER_STATE_IDLE)
    , _lastHeartBeat(0)
{
    x = mx;
    y = my;
}

Player::~Player() {
}

void Player::onEnterScene(int sceneId) {
    _sceneId = sceneId;
    _lastHeartBeat = getNowMs();

    //AoiManager::instance().addObject(_playerId);
    broadcastAppear();

    printf("[Player] %llu enter scene %d\n", _playerId, _sceneId);
    std::cout << "玩家当前的位置是 " << x << "," << y <<std::endl;
    int left = SceneManager::instance().getScene(_sceneId)->getLeftX();
    int right = SceneManager::instance().getScene(_sceneId)->getRightX();
    int top =  SceneManager::instance().getScene(_sceneId)->getTopY();
    int bot = SceneManager::instance().getScene(_sceneId)->getBottonY();

    std::cout << "当前地图的边界点是" << std::endl;
    std::cout << left << "," << top << std::endl; 
    std::cout << right << "," << top << std::endl; 
    std::cout << left << "," << bot << std::endl; 
    std::cout << right << "," << bot << std::endl; 
}

void Player::onLeaveScene() {
    //AoiManager::instance().removeObject(_playerId);
    broadcastDisappear();

    printf("[Player] %llu leave scene %d\n", _playerId, _sceneId);
}

void Player::onDisconnect() {
    onLeaveScene();
    printf("[Player] %llu disconnected\n", _playerId);
}

void Player::update(int deltaMs) {
    // 这里可以更新移动、buff、技能CD等
}

void Player::refreshHeartBeat() {
    _lastHeartBeat = getNowMs();
}

bool Player::isTimeout(uint64_t nowMs) const {
    return nowMs - _lastHeartBeat > 30000;
}

void Player::broadcastAppear() {
    //广播周围玩家出现
}

void Player::broadcastDisappear() {
    //广播周围玩家消失
}

uint64_t Player::getNowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}