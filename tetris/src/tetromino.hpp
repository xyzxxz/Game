#pragma once
#include <array>
#include "types.hpp"

// ============================================================
// 七种标准俄罗斯方块的定义与旋转数据
// 每种方块最多 4 种旋转状态，每状态下包含 4 个格子的相对坐标
// ============================================================

constexpr int kMaxRotations = 4;
constexpr int kCellsPerPiece = 4;

// 单个旋转状态：4 个格子的坐标偏移
using ShapeData = std::array<Point, kCellsPerPiece>;

// 返回指定类型和旋转状态下的方块形状数据
const ShapeData& getShape(TetrominoType type, int rotation);

// 获取某个方块类型可用的旋转次数（O 形只有 1 种）
int rotationCount(TetrominoType type);