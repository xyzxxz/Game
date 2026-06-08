module;

#include <cstdint>
#include <array>
#include <random>
#include <vector>

export module game;

// 所有类型已通过全局模块片段中的 #include 引入

namespace game {

// ---- 方向枚举 ----
export enum class Direction {
    Up,
    Down,
    Left,
    Right
};

// ---- 游戏状态 ----
export enum class GameState {
    Menu,
    Playing,
    Won,
    Lost,
    Quit
};

// ---- 单个格子 ----
export struct Tile {
    int value;      // 0 表示空
};

// ---- 4x4 棋盘 ----
export class Board {
public:
    static constexpr int SIZE = 4;

    Board();

    // 重置棋盘
    void reset();

    // 在空位随机生成一个新方块 (2 或 4)
    void spawnTile();

    // 向某个方向移动，返回是否发生移动
    bool move(Direction dir);

    // 是否还能移动
    bool canMove() const;

    // 是否已达 2048
    bool hasWon() const;

    // 获取格子值
    int  getTile(int row, int col) const;
    void setTile(int row, int col, int value);

    // 获取所有非空格子坐标
    std::vector<std::pair<int, int>> getEmptyTiles() const;

    // 获取当前分数
    int getScore() const;

private:
    std::array<std::array<Tile, SIZE>, SIZE> m_grid;
    std::mt19937 m_rng;
    int m_score;

    bool slideRow(int row, Direction dir);
    bool mergeRow(int row, Direction dir);
};

// ---- 游戏主控 ----
export class Game {
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
