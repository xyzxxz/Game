#pragma once
#include <SDL3/SDL.h>

// ============================================================
// 公共类型、常量和枚举定义
// ============================================================

// ---------- 棋盘尺寸 ----------
constexpr int kBoardCols = 10;      // 棋盘列数
constexpr int kBoardRows = 20;      // 棋盘行数

// ---------- 渲染布局 ----------
constexpr int kCellSize   = 30;     // 每个格子的像素大小
constexpr int kPanelWidth = 180;    // 左侧信息面板宽度
constexpr int kPadding    = 20;     // 四周留白
constexpr int kWindowW    = kPanelWidth + kBoardCols * kCellSize + kPadding * 3;
constexpr int kWindowH    = kBoardRows * kCellSize + kPadding * 2;

// ---------- 棋盘在窗口中的偏移 ----------
constexpr int kBoardX = kPanelWidth + kPadding * 2;
constexpr int kBoardY = kPadding;

// ---------- 方块类型 ----------
enum class TetrominoType { I, O, T, S, Z, J, L };

// ---------- 每种方块的显示颜色（统一使用稍暗白色） ----------
constexpr SDL_Color kTetrominoColors[] = {
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
    { 180, 180, 180, 255 },
};

constexpr SDL_Color kGridColor  = { 40, 40, 40, 255 };   // 网格线颜色
constexpr SDL_Color kEmptyColor = { 15, 15, 15, 255 };   // 空格底色
constexpr SDL_Color kBgColor    = {  0,  0,  0, 255 };   // 窗口背景（纯黑）

// ---------- 坐标点 ----------
struct Point {
    int x, y;
};