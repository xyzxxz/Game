#pragma once
#include <random>
#include "board.hpp"
#include "tetromino.hpp"

class Renderer;

// ============================================================
// 游戏核心逻辑：管理游戏状态、方块移动/旋转、计分与升级
// 同时负责主循环，使得 main 函数保持简洁
// ============================================================

class Game {
public:
    Game();

    // 重置游戏到初始状态
    void reset();

    // ---------- 运行主循环（接管 main 中的逻辑）----------
    void run(Renderer& renderer);

    // ---------- 玩家输入 ----------
    void moveLeft();
    void moveRight();
    bool moveDown();       // 下移一格，成功返回 true
    void rotate();         // 顺时针旋转
    void hardDrop();       // 硬降（直接落底）

    // 每帧更新（自动下落计时）
    void update(float deltaTime);

    // ---------- 状态查询 ----------
    bool isGameOver() const        { return gameOver_; }
    int  score()       const       { return score_; }
    int  level()       const       { return level_; }
    int  linesCleared() const      { return linesCleared_; }

    // 当前活动方块信息（供渲染使用）
    TetrominoType currentType()     const { return curType_; }
    int           currentRotation() const { return curRotation_; }
    int           currentX()        const { return curX_; }
    int           currentY()        const { return curY_; }
    TetrominoType nextType()        const { return nextType_; }

    // 获取棋盘数据（供渲染使用）
    const Board& board() const { return board_; }

    // 获取当前等级的自动下落间隔（秒）
    float dropInterval() const;

private:
    Board board_;                     // 棋盘

    // 当前活动方块
    TetrominoType curType_     = TetrominoType::I;
    int           curRotation_ = 0;
    int           curX_        = 0;   // 方块锚点坐标（棋盘列/行）
    int           curY_        = 0;

    TetrominoType nextType_;         // 下一个方块预览

    int  score_         = 0;         // 得分
    int  level_         = 1;         // 当前等级
    int  linesCleared_  = 0;         // 累计消行数
    bool gameOver_      = false;

    float dropTimer_    = 0.0f;      // 下落计时器

    std::mt19937 rng_;               // 随机数生成器

    // 随机生成一个新方块类型
    TetrominoType randomType();

    // 尝试生成一个新活动方块；若无法放入则游戏结束
    void spawnPiece();

    // 将当前方块锁定到棋盘并处理消行/计分
    void lockPiece();

    // 处理单个按键输入（WASD 控制）
    void handleKey(SDL_Scancode scancode);
};