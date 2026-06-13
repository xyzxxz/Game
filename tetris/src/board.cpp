#include "board.hpp"
#include "tetromino.hpp"
#include <algorithm>

Board::Board()
{
    clear();
}

void Board::clear()
{
    for (int r = 0; r < kBoardRows; ++r)
        for (int c = 0; c < kBoardCols; ++c)
            grid_[r][c] = -1;
}

bool Board::inBounds(int x, int y) const
{
    return x >= 0 && x < kBoardCols && y < kBoardRows;
}

bool Board::canPlace(TetrominoType type, int rotation, int px, int py) const
{
    const auto& shape = getShape(type, rotation);
    for (const auto& cell : shape) {
        int x = px + cell.x;
        int y = py + cell.y;
        // 水平越界或超出底部 → 不能放
        if (x < 0 || x >= kBoardCols || y >= kBoardRows) return false;
        // 在棋盘顶部之上 → 允许（下落过程中的方块）
        if (y < 0) continue;
        // 该位置已被占据 → 不能放
        if (grid_[y][x] != -1) return false;
    }
    return true;
}

void Board::place(TetrominoType type, int rotation, int px, int py)
{
    const auto& shape = getShape(type, rotation);
    int colorIdx = static_cast<int>(type);
    for (const auto& cell : shape) {
        int x = px + cell.x;
        int y = py + cell.y;
        if (y < 0 || y >= kBoardRows || x < 0 || x >= kBoardCols) continue;
        grid_[y][x] = colorIdx;
    }
}

int Board::clearLines()
{
    int cleared = 0;
    int writeRow = kBoardRows - 1; // 从底部往上覆盖

    for (int r = kBoardRows - 1; r >= 0; --r) {
        bool full = true;
        for (int c = 0; c < kBoardCols; ++c) {
            if (grid_[r][c] == -1) { full = false; break; }
        }
        if (full) {
            ++cleared;
            continue; // 跳过该行，不复制
        }
        // 非满行 → 向下搬移到 writeRow
        if (writeRow != r) {
            for (int c = 0; c < kBoardCols; ++c)
                grid_[writeRow][c] = grid_[r][c];
        }
        --writeRow;
    }
    // 清空顶部剩余行
    for (int r = writeRow; r >= 0; --r)
        for (int c = 0; c < kBoardCols; ++c)
            grid_[r][c] = -1;

    return cleared;
}