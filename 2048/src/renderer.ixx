module;

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

export module renderer;

import game;

namespace renderer {

// ---- 颜色 ----
export struct Color {
    uint8_t r, g, b, a;
};

// ---- 主题色板 ----
export namespace Colors {
    inline constexpr Color kBackground   { 0xFA, 0xF8, 0xEF, 0xFF };
    inline constexpr Color kBoardBg      { 0xBB, 0xAD, 0xA0, 0xFF };
    inline constexpr Color kTileDefault  { 0xCD, 0xC1, 0xB4, 0xFF };
    inline constexpr Color kTextDark     { 0x77, 0x6E, 0x65, 0xFF };
    inline constexpr Color kTextLight    { 0xF9, 0xF6, 0xF2, 0xFF };
}

// ---- 菜单项描述 ----
export struct MenuItem {
    const char* label;
};

// ---- 棋盘渲染器 ----
export class BoardRenderer {
public:
    BoardRenderer();

    // 渲染完整棋盘
    void render(SDL_Renderer* renderer, const game::Board& board,
                int windowW, int windowH);

    // 获取/设置动画相关
    bool isAnimating() const;
    void updateAnimation(float dt);

private:
    // 绘制单个方块
    void drawTile(SDL_Renderer* renderer, int value,
                  int x, int y, int tileSize);

    // 根据数值获取方块颜色
    Color getTileColor(int value) const;
    Color getTextColor(int value) const;
};

// ---- 菜单渲染器 ----
export class MenuRenderer {
public:
    MenuRenderer();

    // 渲染菜单界面
    void render(SDL_Renderer* renderer, int selectedIndex,
                int windowW, int windowH);

private:
    static constexpr int kMenuItemCount = 2;
    MenuItem m_items[kMenuItemCount];
};

// ---- HUD 渲染 (分数等) ----
export class HUDRenderer {
public:
    HUDRenderer();

    // 渲染顶部 HUD
    void render(SDL_Renderer* renderer, const game::Game& game,
                int windowW, int windowH);
};

} // namespace renderer
