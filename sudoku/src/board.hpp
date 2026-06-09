#pragma once
#include "cell.hpp"
#include <array>
#include <vector>

class Board
{
public:
    static constexpr int SIZE = 9;  // 9x9 标准数独
    static constexpr int SUB = 3;   // 3x3 宫

    Board();

    // 生成完整终盘（回溯法填满 9x9）
    void generateSolution();

    // 基于终盘挖空生成谜题，difficulty: 0.0 ~ 1.0（挖空比例）
    void generatePuzzle(double difficulty);

    // 单次：同时生成终盘+谜题
    void generate(double difficulty);

    // 访问格子
    Cell* cellAt(int row, int col);
    const Cell* cellAt(int row, int col) const;

    // 尝试在指定位置填入数字，返回是否合法
    bool trySetValue(int row, int col, int value);

    // 擦除指定位置（仅限 FILLED 类型的格子）
    void clearCell(int row, int col);

    // 检查数独是否已填满且合法
    bool isComplete() const;

    // 检查指定位置的当前值是否与同行/列/宫冲突
    bool hasConflict(int row, int col) const;

    // 刷新全盘冲突标记
    void refreshConflicts();

    // 棋盘全部重置
    void reset();

private:
    std::array<std::array<Cell, SIZE>, SIZE> m_grid;

    // 判断在某个位置填 value 是否合法（不计自身）
    bool isValid(int row, int col, int value) const;

    // 回溯填数辅助
    bool solve(int row, int col);

    // 清除所有用户填入的格子
    void clearFilled();
};
