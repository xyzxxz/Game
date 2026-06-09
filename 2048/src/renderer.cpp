#include "renderer.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace renderer {

// ============================================================
//  辅助：将 Color 设为 SDL 绘制颜色
// ============================================================
static void setDrawColor(SDL_Renderer* renderer, Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

// ============================================================
//  BoardRenderer 实现
// ============================================================

BoardRenderer::BoardRenderer() {}

void BoardRenderer::render(SDL_Renderer* renderer, const game::Board& board,
                           TTF_TextEngine* textEngine, TTF_Font* font)
{
    // 计算棋盘区域：居中，留边距，保持正方形
    int boardSize = 520;                         // 棋盘边长
    int padding   = 10;                          // 棋盘内边距
    int gap       = 12;                          // 格子间距
    int tileSize  = (boardSize - 2 * padding - 3 * gap) / 4; // 单格宽度
    int gapTotal  = boardSize - 2 * padding;                  // 含格子+间隙的总区域
    int tileStep  = tileSize + gap;                            // 格子步长

    // 棋盘左上角（居中）
    int boardX = 40;
    int boardY = 140;

    // ---- 绘制棋盘背景 ----
    SDL_FRect boardRect{ (float)boardX, (float)boardY, (float)boardSize, (float)boardSize };
    setDrawColor(renderer, Colors::kBoardBg);
    SDL_RenderFillRect(renderer, &boardRect);

    // ---- 绘制每一个格子 ----
    for (int r = 0; r < game::Board::SIZE; ++r) {
        for (int c = 0; c < game::Board::SIZE; ++c) {
            int value = board.getTile(r, c);
            int x = boardX + padding + c * tileStep;
            int y = boardY + padding + r * tileStep;

            drawTile(renderer, value, x, y, tileSize);

            // 非空格子绘制数字文本
            if (value != 0) {
                std::string text = std::to_string(value);
                TTF_Text* ttfText = TTF_CreateText(textEngine, font, text.c_str(), text.length());
                if (ttfText) {
                    // 计算文本尺寸用于居中
                    int tw = 0, th = 0;
                    TTF_GetTextSize(ttfText, &tw, &th);

                    // 颜色：小数字深色文字、大数字浅色文字
                    Color textColor = getTextColor(value);
                    TTF_SetTextColor(ttfText, textColor.r, textColor.g, textColor.b, textColor.a);
                    TTF_DrawRendererText(ttfText,
                        x + (tileSize - tw) / 2.0f,
                        y + (tileSize - th) / 2.0f);
                    TTF_DestroyText(ttfText);
                }
            }
        }
    }
}

// ---- 绘制单个方块背景 ----
void BoardRenderer::drawTile(SDL_Renderer* renderer, int value,
                             int x, int y, int tileSize)
{
    Color c = getTileColor(value);
    setDrawColor(renderer, c);

    SDL_FRect rect{ (float)x, (float)y, (float)tileSize, (float)tileSize };
    SDL_RenderFillRect(renderer, &rect);
}

// ---- 根据方块数值映射颜色 ----
Color BoardRenderer::getTileColor(int value) const
{
    switch (value) {
    case 0:     return Colors::kTileDefault;
    case 2:     return Color{ 238, 228, 218, 255 };
    case 4:     return Color{ 237, 224, 200, 255 };
    case 8:     return Color{ 242, 177, 121, 255 };
    case 16:    return Color{ 245, 149,  99, 255 };
    case 32:    return Color{ 246, 124,  95, 255 };
    case 64:    return Color{ 246,  94,  59, 255 };
    case 128:   return Color{ 237, 207, 114, 255 };
    case 256:   return Color{ 237, 204,  97, 255 };
    case 512:   return Color{ 237, 200,  80, 255 };
    case 1024:  return Color{ 237, 197,  63, 255 };
    case 2048:  return Color{ 237, 194,  46, 255 };
    default:    return Color{  60,  58,  50, 255 };  // 超 2048
    }
}

// ---- 根据方块数值选择文字颜色 ----
Color BoardRenderer::getTextColor(int value) const
{
    // 8 及以下用深色文字，以上用浅色文字
    return (value <= 8) ? Colors::kTextDark : Colors::kTextLight;
}

bool BoardRenderer::isAnimating() const { return false; }

void BoardRenderer::updateAnimation(float dt) {}

// ============================================================
//  MenuRenderer 实现
// ============================================================

MenuRenderer::MenuRenderer()
{
    m_items[0] = { "1. Start" };
    m_items[1] = { "2. Quit"  };
}

void MenuRenderer::render(SDL_Renderer* renderer, int selectedIndex,
                          TTF_TextEngine* textEngine, TTF_Font* font)
{
    // 背景
    setDrawColor(renderer, Colors::kBackground);
    SDL_RenderClear(renderer);

    // ---- 标题 ----
    {
        TTF_Text* title = TTF_CreateText(textEngine, font, "2048", 4);
        if (title) {
            int tw = 0, th = 0;
            TTF_GetTextSize(title, &tw, &th);
            // 使用卡其色
            TTF_SetTextColor(title, Colors::kTextDark.r, Colors::kTextDark.g,
                             Colors::kTextDark.b, Colors::kTextDark.a);
            TTF_DrawRendererText(title, 300.0f - tw / 2.0f, 100.0f);
            TTF_DestroyText(title);
        }
    }

    // ---- 菜单项 ----
    for (int i = 0; i < kMenuItemCount; ++i) {
        TTF_Text* item = TTF_CreateText(textEngine, font,
                                        m_items[i].label.data(),
                                        m_items[i].label.size());
        if (item) {
            int tw = 0, th = 0;
            TTF_GetTextSize(item, &tw, &th);

            // 当前选中项用高亮色，其余灰色
            if (i == selectedIndex) {
                TTF_SetTextColor(item, 0x77, 0x6E, 0x65, 0xFF);
            } else {
                TTF_SetTextColor(item, 0xBB, 0xBB, 0xBB, 0xFF);
            }

            TTF_DrawRendererText(item,
                300.0f - tw / 2.0f,
                300.0f + i * 60.0f);
            TTF_DestroyText(item);
        }
    }

    // ---- 提示文字 ----
    {
        TTF_Text* hint = TTF_CreateText(textEngine, font, "Press ENTER to confirm", 22);
        if (hint) {
            int tw = 0, th = 0;
            TTF_GetTextSize(hint, &tw, &th);
            TTF_SetTextColor(hint, 0xBB, 0xBB, 0xBB, 0xFF);
            TTF_DrawRendererText(hint, 300.0f - tw / 2.0f, 480.0f);
            TTF_DestroyText(hint);
        }
    }
}

// ============================================================
//  HUDRenderer 实现
// ============================================================

HUDRenderer::HUDRenderer() {}

void HUDRenderer::render(SDL_Renderer* renderer, const game::Game& game,
                         TTF_TextEngine* textEngine, TTF_Font* font)
{
    // ---- 分数 ----
    std::string scoreStr = "Score: " + std::to_string(game.getScore());
    TTF_Text* scoreText = TTF_CreateText(textEngine, font,
                                         scoreStr.c_str(), scoreStr.length());
    if (scoreText) {
        TTF_SetTextColor(scoreText,
                         Colors::kTextDark.r, Colors::kTextDark.g,
                         Colors::kTextDark.b, Colors::kTextDark.a);
        TTF_DrawRendererText(scoreText, 40.0f, 60.0f);
        TTF_DestroyText(scoreText);
    }

    // ---- 游戏结束提示 ----
    const char* stateMsg = nullptr;
    switch (game.getState()) {
    case game::GameState::Won:  stateMsg = "You Win!  Press ESC for menu";  break;
    case game::GameState::Lost: stateMsg = "Game Over!  Press ESC for menu"; break;
    default: break;
    }

    if (stateMsg) {
        TTF_Text* msgText = TTF_CreateText(textEngine, font, stateMsg, 0);
        if (msgText) {
            int tw = 0, th = 0;
            TTF_GetTextSize(msgText, &tw, &th);
            TTF_SetTextColor(msgText, 0xF6, 0x5E, 0x3B, 0xFF);  // 红色提示
            TTF_DrawRendererText(msgText, 40.0f, 100.0f);
            TTF_DestroyText(msgText);
        }
    }
}

} // namespace renderer
