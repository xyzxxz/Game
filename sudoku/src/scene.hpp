#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL3_ttf.h>
#include "board.hpp"
#include "input.hpp"
#include "renderer.hpp"

class Scene
{
public:
    Scene();
    ~Scene();

    // 初始化 SDL 窗口、渲染器和资源
    void init();

    // 生成随机数独新游戏
    void newGame();

    // 检查当前数独是否已完成
    bool isComplete() const;

    // 游戏主循环
    void play();

    // 处理单次输入事件
    void handleInput(const InputEvent& event);

    // 更新选中格子
    void selectCell(int row, int col);

    // 在选中格子填入数字
    void inputNumber(int value);

    // 擦除选中格子
    void eraseCell();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;

    Board    m_board;
    Input    m_input;
    Renderer m_renderer;

    int m_selectedRow = -1;
    int m_selectedCol = -1;
};
