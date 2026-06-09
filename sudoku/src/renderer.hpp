#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL3_ttf.h>
#include "board.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    // 创建窗口和渲染器，返回是否成功
    bool init(SDL_Window*& outWindow, SDL_Renderer*& outRenderer);

    // 渲染完整一帧
    void render(SDL_Renderer* renderer, const Board& board,
                int selectedRow, int selectedCol);

private:
    TTF_Font* m_font = nullptr;

    // ---- 布局常量 ----
    static constexpr int WINDOW_W = 540;
    static constexpr int WINDOW_H = 600;
    static constexpr int BOARD_X = 30;
    static constexpr int BOARD_Y = 30;
    static constexpr int BOARD_SIZE = 480;  // 棋盘总宽
    static constexpr int CELL_SIZE = BOARD_SIZE / 9;

    // ---- 绘制子步骤 ----
    void drawBackground(SDL_Renderer* renderer);
    void drawGrid(SDL_Renderer* renderer);
    void drawCells(SDL_Renderer* renderer, const Board& board,
                   int selectedRow, int selectedCol);
    void drawNumber(SDL_Renderer* renderer, int row, int col, int value,
                    bool isGiven, bool hasError);
    void drawSelection(SDL_Renderer* renderer, int row, int col);
    void drawButtons(SDL_Renderer* renderer);
};
