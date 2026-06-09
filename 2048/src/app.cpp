#include "app.h"
#include <SDL3/SDL_log.h>
#include <print>

namespace app {

// ============================================================
//  App 实现 — 应用生命周期
// ============================================================

App::App()
    : m_menuSelection(MenuOption::Start)
    , m_running(false)
{}

App::~App() {}

int App::run()
{
    // ---- SDL 初始化 ----
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    m_window = SDL_CreateWindow("2048", m_windowW, m_windowH, SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        SDL_Quit();
        return 1;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    if (!TTF_Init()) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    // 加载字体（需将 .ttf 文件放在 exe 同级目录）
    m_font = TTF_OpenFont("font.ttf", 24);
    if (!m_font) {
        TTF_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    // 创建文本渲染引擎
    m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
    if (!m_textEngine) {
        TTF_CloseFont(m_font);
        TTF_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    // ---- 进入主循环 ----
    m_running = true;
    uint64_t lastTick = SDL_GetTicks();
    SDL_Event event;

    while (m_running) {
        uint64_t nowTick = SDL_GetTicks();
        float    dt      = static_cast<float>(nowTick - lastTick) / 1000.0f;
        lastTick = nowTick;

        // 事件处理
        while (SDL_PollEvent(&event)) {
            handleEvent(event);
        }

        // 帧更新
        update(dt);

        // 帧渲染
        render();

        // 限制帧率 ~60FPS
        SDL_Delay(16);
    }

    // ---- 清理资源 ----
    TTF_DestroyRendererTextEngine(m_textEngine);
    TTF_CloseFont(m_font);
    TTF_Quit();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}

// ============================================================
//  事件分发
// ============================================================

void App::handleEvent(const SDL_Event& event)
{
    // 窗口关闭事件 — 任意状态下都可以退出
    if (event.type == SDL_EVENT_QUIT) {
        m_running = false;
        return;
    }

    // 根据游戏状态分发
    switch (m_game.getState()) {
    case game::GameState::Menu:
        handleMenuEvent(event);
        break;
    case game::GameState::Playing:
        handleGameEvent(event);
        break;
    case game::GameState::Won:
    case game::GameState::Lost:
        // 结束状态：ESC 回到菜单
        handleGameOverEvent(event);
        break;
    default:
        break;
    }
}

// ---- 菜单界面按键处理 ----
void App::handleMenuEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        // 上/下切换选中项
        case SDLK_W:
        case SDLK_UP:
            if (m_menuSelection == MenuOption::Quit)
                m_menuSelection = MenuOption::Start;
            break;

        case SDLK_S:
        case SDLK_DOWN:
            if (m_menuSelection == MenuOption::Start)
                m_menuSelection = MenuOption::Quit;
            break;

        // 数字键快速选择
        case SDLK_1:
            m_menuSelection = MenuOption::Start;
            break;
        case SDLK_2:
            m_menuSelection = MenuOption::Quit;
            break;

        // 回车确认当前选项
        case SDLK_RETURN:
        case SDLK_SPACE:
            switch (m_menuSelection) {
            case MenuOption::Start:
                m_game.reset();
                break;
            case MenuOption::Quit:
                m_running = false;
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

// ---- 游戏进行中按键处理 ----
void App::handleGameEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_W:
        case SDLK_UP:
            m_game.move(game::Direction::Up);
            break;
        case SDLK_S:
        case SDLK_DOWN:
            m_game.move(game::Direction::Down);
            break;
        case SDLK_A:
        case SDLK_LEFT:
            m_game.move(game::Direction::Left);
            break;
        case SDLK_D:
        case SDLK_RIGHT:
            m_game.move(game::Direction::Right);
            break;
        case SDLK_ESCAPE:
            m_game.setState(game::GameState::Menu);
            m_menuSelection = MenuOption::Start;
            break;
        default:
            break;
        }
    }
}

// ---- 游戏结束 / 获胜界面按键处理 ----
void App::handleGameOverEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
        m_game.setState(game::GameState::Menu);
        m_menuSelection = MenuOption::Start;
    }
}

// ============================================================
//  帧更新
// ============================================================
void App::update(float dt)
{
    // 预留：动画更新等
    (void)dt;
}

// ============================================================
//  帧渲染
// ============================================================

void App::render()
{
    // 清屏背景
    SDL_SetRenderDrawColor(m_renderer,
        renderer::Colors::kBackground.r,
        renderer::Colors::kBackground.g,
        renderer::Colors::kBackground.b,
        renderer::Colors::kBackground.a);
    SDL_RenderClear(m_renderer);

    switch (m_game.getState()) {
    case game::GameState::Menu:
        // 菜单界面
        m_menuRenderer.render(m_renderer,
            static_cast<int>(m_menuSelection),
            m_textEngine, m_font);
        break;

    case game::GameState::Playing:
    case game::GameState::Won:
    case game::GameState::Lost:
        // 游戏界面：HUD + 棋盘
        m_hudRenderer.render(m_renderer, m_game, m_textEngine, m_font);
        m_boardRenderer.render(m_renderer, m_game.getBoard(),
                               m_textEngine, m_font);
        break;

    default:
        break;
    }

    SDL_RenderPresent(m_renderer);
}

} // namespace app
