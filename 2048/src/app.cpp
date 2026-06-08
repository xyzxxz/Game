module;

#include <SDL3/SDL.h>

module app;

namespace app {

// ---- App ----
App::App()
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_windowW(800)
    , m_windowH(600)
    , m_menuSelection(MenuOption::Start)
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

    // ---- 主循环 ----
    m_running = true;
    uint64_t lastTick = SDL_GetTicks();
    SDL_Event event;

    while (m_running) {
        uint64_t nowTick = SDL_GetTicks();
        float    dt      = (float)(nowTick - lastTick) / 1000.0f;
        lastTick = nowTick;

        // 事件处理
        while (SDL_PollEvent(&event)) {
            handleEvent(event);
        }

        // 帧更新
        update(dt);

        // 帧渲染
        render();
    }

    // ---- 清理 ----
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}

void App::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_QUIT) {
        m_running = false;
        return;
    }

    switch (m_game.getState()) {
    case game::GameState::Menu:
        handleMenuEvent(event);
        break;
    case game::GameState::Playing:
        handleGameEvent(event);
        break;
    default:
        break;
    }
}

void App::handleMenuEvent(const SDL_Event& event) {}

void App::handleGameEvent(const SDL_Event& event) {}

void App::update(float dt) {}

void App::render()
{
    SDL_SetRenderDrawColor(m_renderer,
        renderer::Colors::kBackground.r,
        renderer::Colors::kBackground.g,
        renderer::Colors::kBackground.b,
        renderer::Colors::kBackground.a);
    SDL_RenderClear(m_renderer);

    switch (m_game.getState()) {
    case game::GameState::Menu:
        m_menuRenderer.render(m_renderer, (int)m_menuSelection, m_windowW, m_windowH);
        break;
    case game::GameState::Playing:
    case game::GameState::Won:
    case game::GameState::Lost:
        m_hudRenderer.render(m_renderer, m_game, m_windowW, m_windowH);
        m_boardRenderer.render(m_renderer, m_game.getBoard(), m_windowW, m_windowH);
        break;
    default:
        break;
    }

    SDL_RenderPresent(m_renderer);
}

} // namespace app
