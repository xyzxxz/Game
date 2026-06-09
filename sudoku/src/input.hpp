#pragma once
#include <SDL3/SDL.h>

// 输入事件抽象
struct InputEvent
{
    enum class Type
    {
        NONE,
        QUIT,
        CELL_CLICKED,   // 点击了某格，附带 row / col
        KEY_NUMBER,     // 按下数字键 1~9，附带 value
        KEY_DELETE,     // 删除 / 退格
        KEY_NEW_GAME,   // 新游戏
        KEY_UNDO,       // 撤销
        KEY_HINT,       // 提示
    };

    Type type = Type::NONE;
    int row = -1;
    int col = -1;
    int value = 0;
};

class Input
{
public:
    Input();

    // 处理所有待处理事件，返回下一个有效游戏事件
    InputEvent poll();

    // 检查是否有退出事件
    bool shouldQuit() const;

private:
    bool m_quit = false;

    // 将鼠标像素坐标转换为棋盘行列，成功返回 true
    bool screenToBoard(float mouseX, float mouseY, int& outRow, int& outCol) const;
};
