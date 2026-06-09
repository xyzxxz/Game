#pragma once

#include <array>
#include <random>
#include <vector>

namespace game {

// 滑动方向
enum class Direction {
    Up,
    Down,
    Left,
    Right
};

// 游戏状态
enum class GameState {
    Menu,
    Playing,
    Won,
    Lost,
    Quit
};

// 单个格子，value 为 0 表示空
struct Tile {
    int value;
};

// 4x4 棋盘，核心游戏逻辑
class Board {
public:
    static constexpr int SIZE = 4;

    Board();

    // 重置棋盘为初始状态，生成两个方块
    void reset();

    // 在随机空位生成 2（90%）或 4（10%）
    void spawnTile();

    // 向指定方向移动，返回是否有方块移动
    bool move(Direction dir);

    // 判断是否还有合法移动
    bool canMove() const;

    // 是否达到 2048
    bool hasWon() const;

    // 访问格子
    int  getTile(int row, int col) const;
    void setTile(int row, int col, int value);

    // 获取所有空位坐标
    std::vector<std::pair<int, int>> getEmptyTiles() const;

    // 当前分数
    int getScore() const;

private:
    std::array<std::array<Tile, SIZE>, SIZE> m_grid;
    std::mt19937 m_rng;
    int m_score;

    // 处理一行（4 个值），向索引 0 方向滑动 + 合并，返回是否变化
    static bool processLine(int line[SIZE], int& scoreDelta);
};

// 游戏主控，管理游戏进程和状态切换
class Game {
public:
    Game();

    // 重置整局游戏
    void reset();

    // 执行一步移动
    void move(Direction dir);

    // 状态查询
    GameState   getState()   const;
    Board&      getBoard();
    const Board& getBoard()  const;
    int         getScore()   const;

    // 设置游戏状态
    void setState(GameState state);

private:
    Board     m_board;
    GameState m_state;
};

} // namespace game
