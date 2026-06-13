#include "tetromino.hpp"

// ============================================================
// 七种方块在所有旋转状态下的格子坐标偏移
// shapes[type][rotation] = { Point(x1,y1), ..., Point(x4,y4) }
// ============================================================

// I 形（4 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeI = {{
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }}},   // 0°
    {{{ 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 }}},   // 90°
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }}},   // 180°
    {{{ 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 }}},   // 270°
}};

// O 形（1 个旋转状态，其余三个与 0 相同）
constexpr std::array<ShapeData, kMaxRotations> kShapeO = {{
    {{{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }}},
    {{{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }}},
    {{{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }}},
    {{{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }}},
}};

// T 形（4 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeT = {{
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 }}},   // 0°
    {{{ 0, -1 }, { 0, 0 }, { 1, 0 }, { 0, 1 }}},   // 90°
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, -1 }}},  // 180°
    {{{ 0, -1 }, { -1, 0 }, { 0, 0 }, { 0, 1 }}},  // 270°
}};

// S 形（2 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeS = {{
    {{{ 0, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }}},   // 0°
    {{{ 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 }}},   // 90°
    {{{ 0, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }}},   // 180°
    {{{ 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 }}},   // 270°
}};

// Z 形（2 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeZ = {{
    {{{ -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 }}},   // 0°
    {{{ 1, -1 }, { 0, 0 }, { 1, 0 }, { 0, 1 }}},   // 90°
    {{{ -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 }}},   // 180°
    {{{ 1, -1 }, { 0, 0 }, { 1, 0 }, { 0, 1 }}},   // 270°
}};

// J 形（4 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeJ = {{
    {{{ -1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 }}}, // 0°
    {{{ 0, -1 }, { 1, -1 }, { 0, 0 }, { 0, 1 }}},  // 90°
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 }}},   // 180°
    {{{ 0, -1 }, { 0, 0 }, { -1, 1 }, { 0, 1 }}},  // 270°
}};

// L 形（4 个旋转状态）
constexpr std::array<ShapeData, kMaxRotations> kShapeL = {{
    {{{ 1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 }}},  // 0°
    {{{ 0, -1 }, { 0, 0 }, { 0, 1 }, { 1, 1 }}},   // 90°
    {{{ -1, 0 }, { 0, 0 }, { 1, 0 }, { -1, 1 }}},  // 180°
    {{{ -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 }}}, // 270°
}};

// 汇总所有形状
constexpr std::array<std::array<ShapeData, kMaxRotations>, 7> kAllShapes = {
    kShapeI, kShapeO, kShapeT, kShapeS, kShapeZ, kShapeJ, kShapeL
};

const ShapeData& getShape(TetrominoType type, int rotation)
{
    int idx  = static_cast<int>(type);
    int rot  = rotation % kMaxRotations;
    return kAllShapes[idx][rot];
}

int rotationCount(TetrominoType type)
{
    // O 形只有一个有效旋转
    if (type == TetrominoType::O) return 1;
    return kMaxRotations;
}