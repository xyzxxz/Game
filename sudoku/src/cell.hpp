#pragma once
#include <cstdint>

// 格子的三种状态
enum class CellState : uint8_t
{
    EMPTY,  // 空格，等待用户填入
    GIVEN,  // 题目给出的固定数字（不可修改）
    FILLED  // 用户填入的数字（可修改）
};

struct Cell
{
    int value = 0;          // 数字，0 表示空
    CellState state = CellState::EMPTY;
    bool selected = false;  // 是否被玩家选中（高亮）
    bool hasError = false;  // 是否与同行/同列/同宫冲突
};
