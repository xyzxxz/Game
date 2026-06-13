#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "game.hpp"

// ============================================================
// 渲染模块：负责所有 SDL3 绘制工作
// 包括棋盘、当前/预览方块、得分面板等
// ============================================================

class Renderer {
public:
    Renderer();
    ~Renderer();

    // 初始化窗口、渲染器和字体，成功返回 true
    bool init();

    // 绘制完整一帧
    void render(const Game& game);

private:
    SDL_Window*   window_   = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font*     font_     = nullptr;

    // ---------- 绘制子步骤 ----------
    void drawBackground();
    void drawBoard(const Board& board);
    void drawGhost(const Game& game);                              // 预览落点
    void drawPiece(TetrominoType type, int rotation, int px, int py); // 以棋盘坐标绘制
    void drawPanel(const Game& game);                              // 左侧面板
    void drawNextPreview(const Game& game);                        // 下一个方块

    // ---------- 辅助 ----------
    void drawCell(int screenX, int screenY, SDL_Color color);     // 绘制一个格子
    void drawText(const char* text, int x, int y, SDL_Color color); // 绘制文字
};