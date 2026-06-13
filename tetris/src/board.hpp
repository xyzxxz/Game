#pragma once
#include <array>
#include "types.hpp"

// ============================================================
// 棋盘管理：10×20 的网格，处理碰撞检测、方块锁定、消行
// ============================================================

class Board {
public:
    Board();

    // 重置棋盘（全空）
    void clear();

    // 检查指定位置是否可以放置某方块（无碰撞、不越界）
    bool canPlace(TetrominoType type, int rotation, int px, int py) const;

    // 将方块锁定到棋盘上
    void place(TetrominoType type, int rotation, int px, int py);

    // 检查并清除满行，返回消除的行数
    int clearLines();

    // 获取指定格子的类型，-1 表示空格
    int cell(int row, int col) const { return grid_[row][col]; }

private:
    // -1 = 空，0~6 = 对应 TetrominoType
    int grid_[kBoardRows][kBoardCols];

    // 检查坐标是否在棋盘范围内
    bool inBounds(int x, int y) const;
};