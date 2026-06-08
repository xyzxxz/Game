#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <algorithm>

// ── 常量 ──────────────────────────────────────────────

constexpr int kWinW{700};
constexpr int kWinH{700};
constexpr int kHeader{44};               // 顶部信息栏高度
constexpr int kGrid{20};                 // 网格单元大小
constexpr int kFPS{120};                 // 渲染帧率上限
constexpr int kFrameMs{1000 / kFPS};     // 每帧预算（毫秒）
constexpr int kSlow{200};                // 蛇初始移动间隔（毫秒）
constexpr int kFast{50};                 // 蛇最快移动间隔（毫秒）

enum class Dir { Up, Down, Left, Right };
enum class State { Playing, Paused, Over };

// ── 渲染器 ────────────────────────────────────────────

class Renderer {
public:
    bool init() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init: %s", SDL_GetError());
            return false;
        }
        win = SDL_CreateWindow("Snake", kWinW, kWinH, SDL_WINDOW_RESIZABLE);
        if (!win) return false;
        ren = SDL_CreateRenderer(win, nullptr);
        if (!ren) return false;

        if (TTF_Init()) {
            auto* font = TTF_OpenFont("font.ttf", 32);
            auto* fontBig = TTF_OpenFont("font.ttf", 56);
            if (font) {
                auto* engine = TTF_CreateRendererTextEngine(ren);
                if (engine) {
                    txtScore = TTF_CreateText(engine, font, "Score: 0", 0);
                    txtFPS   = TTF_CreateText(engine, font, "FPS: 0", 0);
                    txtOver  = TTF_CreateText(engine, fontBig, "Game Over", 0);
                    if (txtScore) TTF_SetTextColor(txtScore, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    if (txtFPS)   TTF_SetTextColor(txtFPS,   200, 200, 200, SDL_ALPHA_OPAQUE);
                    if (txtOver)  TTF_SetTextColor(txtOver,  255,  50,  50, SDL_ALPHA_OPAQUE);
                }
            }
        }
        return true;
    }

    void draw(State state, int score, int fps,
              const std::vector<glm::vec2>& snake, glm::vec2 food) {
        // 清屏
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);

        // 顶部信息栏分隔线
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        SDL_RenderLine(ren, 0, kHeader, kWinW, kHeader);

        // 裁剪到游戏区域（header 以下）
        SDL_Rect clip = {0, kHeader, kWinW, kWinH - kHeader};
        SDL_SetRenderClipRect(ren, &clip);

        // 网格线
        SDL_SetRenderDrawColor(ren, 35, 35, 35, 255);
        for (int x = 0; x <= kWinW; x += kGrid)
            SDL_RenderLine(ren, x, kHeader, x, kWinH);
        for (int y = kHeader; y <= kWinH; y += kGrid)
            SDL_RenderLine(ren, 0, y, kWinW, y);

        // 蛇身
        SDL_SetRenderDrawColor(ren, 0, 220, 0, 255);
        SDL_FRect rect;
        for (auto& s : snake) {
            rect = {s.x, s.y + kHeader, kGrid, kGrid};
            SDL_RenderFillRect(ren, &rect);
        }

        // 食物
        SDL_SetRenderDrawColor(ren, 255, 80, 80, 255);
        rect = {food.x, food.y + kHeader, kGrid, kGrid};
        SDL_RenderFillRect(ren, &rect);

        SDL_SetRenderClipRect(ren, nullptr);  // 解除裁剪

        // 左上角：分数（始终可见）
        if (txtScore) {
            TTF_SetTextString(txtScore,
                (state == State::Over
                    ? "Score: " + std::to_string(score) + "  [Over]"
                    : "Score: " + std::to_string(score)).c_str(), 0);
            TTF_DrawRendererText(txtScore, 10.0f, 6.0f);
        }

        // 右上角：帧率
        if (txtFPS) {
            TTF_SetTextString(txtFPS, ("FPS: " + std::to_string(fps)).c_str(), 0);
            int tw, th;
            TTF_GetTextSize(txtFPS, &tw, &th);
            TTF_DrawRendererText(txtFPS, (float)(kWinW - tw - 10), 6.0f);
        }

        // Game Over（居中大字）
        if (txtOver && state == State::Over) {
            int tw, th;
            TTF_GetTextSize(txtOver, &tw, &th);
            TTF_DrawRendererText(txtOver, (kWinW - tw) / 2.0f,
                                 kHeader + (kWinH - kHeader - th) / 2.0f);
        }

        SDL_RenderPresent(ren);
    }

    void destroy() {
        TTF_Quit();
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

private:
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    TTF_Text* txtScore = nullptr;
    TTF_Text* txtFPS = nullptr;
    TTF_Text* txtOver = nullptr;
};

// ── 游戏逻辑 ──────────────────────────────────────────

class Game {
public:
    static Game& me() { static Game g; return g; }

    int run() {
        if (!gfx.init()) return 1;

        int frames = 0, fps = 0;
        auto fpsTimer = SDL_GetTicks();

        while (true) {
            auto t0 = SDL_GetTicks();

            // ── 输入处理 ──
            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_EVENT_QUIT) return 0;
                if (ev.type != SDL_EVENT_KEY_DOWN) continue;

                // 暂停/继续
                if (ev.key.key == SDLK_SPACE) {
                    if (state == State::Playing) state = State::Paused;
                    else if (state == State::Paused) state = State::Playing;
                    continue;
                }
                if (state != State::Playing) continue;

                // 方向控制（禁止反向）
                Dir next = dir;
                if (ev.key.key == SDLK_W && dir != Dir::Down)  next = Dir::Up;
                if (ev.key.key == SDLK_S && dir != Dir::Up)    next = Dir::Down;
                if (ev.key.key == SDLK_A && dir != Dir::Right) next = Dir::Left;
                if (ev.key.key == SDLK_D && dir != Dir::Left)  next = Dir::Right;
                dir = next;
            }

            // ── 逻辑更新（固定间隔） ──
            int interval = std::max(kFast, kSlow - (int)segments.size() * 8);
            if (state == State::Playing && t0 - moveTimer >= interval) {
                tick();
                moveTimer = t0;
            }

            // ── 渲染 ──
            gfx.draw(state, score, fps, segments, food);

            // ── FPS 统计（每 500ms） ──
            frames++;
            if (t0 - fpsTimer >= 500) {
                fps = (int)(frames * 1000.0f / (t0 - fpsTimer));
                frames = 0;
                fpsTimer = t0;
            }

            // 帧率限制
            auto cost = SDL_GetTicks() - t0;
            if (cost < kFrameMs)
                SDL_Delay((Uint32)(kFrameMs - cost));
        }
        gfx.destroy();
        return 0;
    }

private:
    Game() { reset(); }

    void reset() {
        score = 0; dir = Dir::Up;
        // 计算游戏区域网格尺寸，蛇头居中并对齐网格
        int gw = (kWinW / kGrid) * kGrid;
        int gh = ((kWinH - kHeader) / kGrid) * kGrid;
        segments = {{(float)((gw / kGrid / 2) * kGrid),
                     (float)((gh / kGrid / 2) * kGrid)}};
        state = State::Playing;
        spawnFood();
    }

    void tick() {
        // 蛇身移位：尾部跟随前一节
        glm::vec2 tail = segments.back();
        for (size_t i = segments.size() - 1; i > 0; --i)
            segments[i] = segments[i - 1];

        // 蛇头按方向移动一格
        switch (dir) {
            case Dir::Up:    segments[0].y -= kGrid; break;
            case Dir::Down:  segments[0].y += kGrid; break;
            case Dir::Left:  segments[0].x -= kGrid; break;
            case Dir::Right: segments[0].x += kGrid; break;
        }

        // 碰撞检测
        auto& head = segments[0];
        int gw = (kWinW / kGrid) * kGrid;
        int gh = ((kWinH - kHeader) / kGrid) * kGrid;
        if (head.x < 0 || head.x >= gw || head.y < 0 || head.y >= gh) {
            state = State::Over; return;
        }
        for (size_t i = 1; i < segments.size(); ++i)
            if (head == segments[i]) { state = State::Over; return; }

        // 吃到食物：加分 + 增长
        if (head == food) {
            score++;
            spawnFood();
            segments.push_back(tail);
        }
    }

    void spawnFood() {
        int gw = kWinW / kGrid;
        int gh = (kWinH - kHeader) / kGrid;
        do {
            food = {(float)(SDL_rand(gw) * kGrid),
                    (float)(SDL_rand(gh) * kGrid)};
        } while (std::ranges::find(segments, food) != segments.end());
    }

    Renderer gfx;
    State state{State::Playing};
    Dir dir{Dir::Up};
    std::vector<glm::vec2> segments;    // 蛇身各段（[0]为头）
    glm::vec2 food;
    int score = 0;
    Uint64 moveTimer = 0;               // 上次移动时间戳
};

int main() { return Game::me().run(); }
