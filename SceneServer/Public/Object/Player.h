#pragma once
#include "GameObject.h"
#include "../Scene/Aoi.h"

enum PlayerState {
    PLAYER_STATE_IDLE,
    PLAYER_STATE_MOVING,
    PLAYER_STATE_FIGHTING,
    PLAYER_STATE_DEAD
};

class Player : public GameObject {
public:
    explicit Player(uint64_t playerId);
    ~Player() override;

    // 生命周期
    void onEnterScene(int sceneId);
    void onLeaveScene();
    void onDisconnect();
    void update(int deltaMs) override;

    // 心跳
    void refreshHeartBeat();
    bool isTimeout(uint64_t nowMs) const;

    // 广播
    void broadcastAppear();
    void broadcastDisappear();

private:
    uint64_t _playerId;
    int _sceneId;
    PlayerState _state;
    uint64_t _lastHeartBeat;
};