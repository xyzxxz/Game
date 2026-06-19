#pragma once

// ========== 窗口设置 ==========
constexpr int   kScreenWidth  = 288;
constexpr int   kScreenHeight = 512;
constexpr float kLogicalWidth  = 288.0f;
constexpr float kLogicalHeight = 512.0f;

// ========== 物理参数 ==========
constexpr float kGravity        = 980.0f;
constexpr float kFlapVelocity   = -350.0f;
constexpr float kMaxFallSpeed   = 600.0f;
constexpr float kBirdRotationMax = 25.0f;

// ========== 管道参数 ==========
constexpr float kPipeSpeed      = 150.0f;
constexpr float kPipeWidth      = 52.0f;
constexpr float kPipeGap        = 100.0f;
constexpr float kPipeSpawnInterval = 1.8f;

// ========== 地面参数 ==========
constexpr float kGroundSpeed    = 150.0f;
constexpr float kGroundHeight   = 112.0f;
constexpr float kGroundY        = kScreenHeight - kGroundHeight;

// ========== 动画帧率 ==========
constexpr float kBirdAnimInterval = 0.15f;

// ========== 游戏状态 ==========
enum class GameState {
    Menu,
    Playing,
    GameOver
};