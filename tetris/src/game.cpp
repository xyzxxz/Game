#include "game.hpp"
#include "renderer.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

// 分数表：按消除行数 1~4 的得分（乘以等级）
constexpr int kScoreTable[] = { 0, 100, 300, 500, 800 };

// 每升一级需要的消行数
constexpr int kLinesPerLevel = 10;

Game::Game()
{
    rng_.seed(std::random_device{}());
    reset();
}

// ============================================================
// 主循环：处理事件 → 更新状态 → 渲染，约 60 FPS
// ============================================================
void Game::run(Renderer& renderer)
{
    bool   running  = true;
    Uint64 lastTick = SDL_GetTicks();

    while (running) {
        // 计算帧间隔（秒）
        Uint64 now       = SDL_GetTicks();
        float  deltaTime = (now - lastTick) / 1000.0f;
        lastTick         = now;

        // 处理事件
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                handleKey(event.key.scancode);
            }
        }

        // 更新游戏状态
        update(deltaTime);

        // 渲染一帧
        renderer.render(*this);

        SDL_Delay(1);
    }
}

// ============================================================
// WASD 按键处理
// ============================================================
void Game::handleKey(SDL_Scancode scancode)
{
    switch (scancode) {
    case SDL_SCANCODE_A: moveLeft();  break;   // A = 左移
    case SDL_SCANCODE_D: moveRight(); break;   // D = 右移
    case SDL_SCANCODE_S: moveDown();  break;   // S = 下移
    case SDL_SCANCODE_W: rotate();    break;   // W = 旋转
    case SDL_SCANCODE_SPACE: hardDrop(); break; // 空格 = 硬降
    case SDL_SCANCODE_R:
        if (isGameOver()) reset();
        break;
    default: break;
    }
}

void Game::reset()
{
    board_.clear();
    score_        = 0;
    level_        = 1;
    linesCleared_ = 0;
    gameOver_     = false;
    dropTimer_    = 0.0f;

    nextType_ = randomType();
    spawnPiece();
}

TetrominoType Game::randomType()
{
    std::uniform_int_distribution<int> dist(0, 6);
    return static_cast<TetrominoType>(dist(rng_));
}

void Game::spawnPiece()
{
    // 取出预设的下一个方块，再随机生成新的预览方块
    curType_     = nextType_;
    curRotation_ = 0;
    nextType_    = randomType();

    // I 方块初始列偏移不同于其他方块
    curX_ = (curType_ == TetrominoType::I) ? 3 : 3;
    curY_ = -1; // 从棋盘上方出现

    // 如果出生位置已被占据 → 游戏结束
    if (!board_.canPlace(curType_, curRotation_, curX_, curY_)) {
        gameOver_ = true;
    }
}

void Game::moveLeft()
{
    if (board_.canPlace(curType_, curRotation_, curX_ - 1, curY_))
        --curX_;
}

void Game::moveRight()
{
    if (board_.canPlace(curType_, curRotation_, curX_ + 1, curY_))
        ++curX_;
}

bool Game::moveDown()
{
    if (board_.canPlace(curType_, curRotation_, curX_, curY_ + 1)) {
        ++curY_;
        return true;
    }
    lockPiece();
    return false;
}

void Game::rotate()
{
    int maxRot  = rotationCount(curType_);
    int newRot  = (curRotation_ + 1) % maxRot;
    if (board_.canPlace(curType_, newRot, curX_, curY_))
        curRotation_ = newRot;
}

void Game::hardDrop()
{
    // 一直下移直到无法继续
    while (board_.canPlace(curType_, curRotation_, curX_, curY_ + 1))
        ++curY_;
    lockPiece();
}

void Game::update(float deltaTime)
{
    if (gameOver_) return;

    dropTimer_ += deltaTime;
    float interval = dropInterval();
    while (dropTimer_ >= interval) {
        dropTimer_ -= interval;
        moveDown();
        if (gameOver_) return;
    }
}

float Game::dropInterval() const
{
    // 等级越高掉落越快，最少不低于 0.05 秒
    float sec = 0.8f - (level_ - 1) * 0.07f;
    return std::max(sec, 0.05f);
}

void Game::lockPiece()
{
    if (gameOver_) return;

    board_.place(curType_, curRotation_, curX_, curY_);

    // 消行与计分
    int cleared = board_.clearLines();
    if (cleared > 0 && cleared <= 4) {
        score_        += kScoreTable[cleared] * level_;
        linesCleared_ += cleared;
        level_         = 1 + linesCleared_ / kLinesPerLevel;
    }

    // 生成下一个方块
    spawnPiece();
}