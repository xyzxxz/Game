#include "renderer.hpp"
#include "tetromino.hpp"
#include <cstdio>

// 预览区每个格子的缩小比例
constexpr float kPreviewScale = 0.6f;
constexpr int   kPreviewCell  = static_cast<int>(kCellSize * kPreviewScale);

Renderer::Renderer()  = default;
Renderer::~Renderer()
{
    if (font_)     TTF_CloseFont(font_);
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_)   SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}

bool Renderer::init()
{
    // 初始化 SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init 失败: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init()) {
        SDL_Log("TTF_Init 失败: %s", SDL_GetError());
        return false;
    }

    // 创建窗口
    window_ = SDL_CreateWindow("俄罗斯方块 - Tetris",
                               kWindowW, kWindowH,
                               SDL_WINDOW_RESIZABLE);
    if (!window_) {
        SDL_Log("SDL_CreateWindow 失败: %s", SDL_GetError());
        return false;
    }

    // 创建渲染器
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        SDL_Log("SDL_CreateRenderer 失败: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    // 加载项目目录下的字体文件
    font_ = TTF_OpenFont("font.ttf", 24);
    if (!font_) {
        SDL_Log("警告：无法加载 font.ttf，文字将不显示");
    }

    return true;
}

// ============================================================
// 主渲染入口
// ============================================================
void Renderer::render(const Game& game)
{
    drawBackground();
    drawBoard(game.board());
    drawGhost(game);
    drawPiece(game.currentType(), game.currentRotation(), game.currentX(), game.currentY());
    drawPanel(game);
    SDL_RenderPresent(renderer_);
}

// ============================================================
// 绘制窗口背景
// ============================================================
void Renderer::drawBackground()
{
    SDL_SetRenderDrawColor(renderer_, kBgColor.r, kBgColor.g, kBgColor.b, kBgColor.a);
    SDL_RenderClear(renderer_);
}

// ============================================================
// 绘制棋盘网格
// ============================================================
void Renderer::drawBoard(const Board& board)
{
    for (int r = 0; r < kBoardRows; ++r) {
        for (int c = 0; c < kBoardCols; ++c) {
            int sx = kBoardX + c * kCellSize;
            int sy = kBoardY + r * kCellSize;

            int cellType = board.cell(r, c);
            if (cellType >= 0) {
                drawCell(sx, sy, kTetrominoColors[cellType]);
            } else {
                drawCell(sx, sy, kEmptyColor);
            }
            // 网格线
            SDL_FRect rect = { static_cast<float>(sx), static_cast<float>(sy),
                               static_cast<float>(kCellSize), static_cast<float>(kCellSize) };
            SDL_SetRenderDrawColor(renderer_, kGridColor.r, kGridColor.g, kGridColor.b, kGridColor.a);
            SDL_RenderRect(renderer_, &rect);
        }
    }
}

// ============================================================
// 绘制当前活动方块
// ============================================================
void Renderer::drawPiece(TetrominoType type, int rotation, int px, int py)
{
    const auto& shape = getShape(type, rotation);
    SDL_Color color = kTetrominoColors[static_cast<int>(type)];

    for (const auto& cell : shape) {
        int x = px + cell.x;
        int y = py + cell.y;
        if (y < 0) continue; // 在棋盘上方不绘制
        int sx = kBoardX + x * kCellSize;
        int sy = kBoardY + y * kCellSize;
        drawCell(sx, sy, color);
    }
}

// ============================================================
// 绘制幽灵方块（预览落点）
// ============================================================
void Renderer::drawGhost(const Game& game)
{
    if (game.isGameOver()) return;

    int gx = game.currentX();
    int gy = game.currentY();
    // 下移直到碰到底部
    Board tempBoard = game.board(); // 复制棋盘用于碰撞检测
    while (tempBoard.canPlace(game.currentType(), game.currentRotation(), gx, gy + 1))
        ++gy;

    if (gy == game.currentY()) return; // 已经在底部

    const auto& shape = getShape(game.currentType(), game.currentRotation());
    SDL_Color ghostColor = kTetrominoColors[static_cast<int>(game.currentType())];
    ghostColor.a = 60; // 半透明

    for (const auto& cell : shape) {
        int x = gx + cell.x;
        int y = gy + cell.y;
        if (y < 0) continue;
        int sx = kBoardX + x * kCellSize;
        int sy = kBoardY + y * kCellSize;
        drawCell(sx, sy, ghostColor);
    }
}

// ============================================================
// 绘制左侧信息面板
// ============================================================
void Renderer::drawPanel(const Game& game)
{
    SDL_Color textColor = { 220, 220, 220, 255 };
    char buf[64];

    int leftX = kPadding;
    int y     = kPadding + 5;

    // 得分
    snprintf(buf, sizeof(buf), "得  分: %d", game.score());
    drawText(buf, leftX, y, textColor);
    y += 40;

    // 等级
    snprintf(buf, sizeof(buf), "等  级: %d", game.level());
    drawText(buf, leftX, y, textColor);
    y += 40;

    // 消行数
    snprintf(buf, sizeof(buf), "消  行: %d", game.linesCleared());
    drawText(buf, leftX, y, textColor);
    y += 60;

    // 下一个方块标题
    drawText("下一个:", leftX, y, textColor);
    y += 30;

    // 游戏结束提示
    if (game.isGameOver()) {
        SDL_Color warnColor = { 255, 80, 80, 255 };
        drawText("游戏结束!", leftX, y + 80, warnColor);
        drawText("按 R 重新开始", leftX, y + 115, warnColor);
    }

    // 预览下一个方块
    drawNextPreview(game);
}

void Renderer::drawNextPreview(const Game& game)
{
    const auto& shape = getShape(game.nextType(), 0);
    SDL_Color color = kTetrominoColors[static_cast<int>(game.nextType())];

    // 预览区在左侧面板居中显示
    int centerX = kPadding + kPanelWidth / 2;
    int centerY = kPadding + 225;

    for (const auto& cell : shape) {
        int sx = centerX + cell.x * kPreviewCell;
        int sy = centerY + cell.y * kPreviewCell;
        SDL_FRect rect = { static_cast<float>(sx), static_cast<float>(sy),
                           static_cast<float>(kPreviewCell - 1),
                           static_cast<float>(kPreviewCell - 1) };
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer_, &rect);
    }
}

// ============================================================
// 绘制单个格子
// ============================================================
void Renderer::drawCell(int screenX, int screenY, SDL_Color color)
{
    SDL_FRect inner = {
        static_cast<float>(screenX + 1),
        static_cast<float>(screenY + 1),
        static_cast<float>(kCellSize - 2),
        static_cast<float>(kCellSize - 2)
    };
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer_, &inner);
}

// ============================================================
// 绘制文字
// ============================================================
void Renderer::drawText(const char* text, int x, int y, SDL_Color color)
{
    if (!font_) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font_, text, 0, color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    if (texture) {
        SDL_FRect dst = { static_cast<float>(x), static_cast<float>(y),
                          static_cast<float>(surface->w), static_cast<float>(surface->h) };
        SDL_RenderTexture(renderer_, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_DestroySurface(surface);
}