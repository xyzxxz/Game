#include "game.hpp"

namespace core {

Game::Game(std::string_view title, int width, int height)
    : m_window{title, width, height}
    , m_renderer{m_window.handle()}
    , m_last_time{std::chrono::high_resolution_clock::now()}
{
}

int Game::run()
{
    on_init();

    while (m_running) {
        auto now = std::chrono::high_resolution_clock::now();
        m_delta_time = std::chrono::duration<float>(now - m_last_time).count();
        m_last_time = now;

        process_events();

        if (!m_running) break;

        on_update(m_delta_time);
        on_render();
    }

    on_cleanup();
    return 0;
}

void Game::process_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
            return;
        }
        on_event(event);
    }
}

Window& Game::window() noexcept { return m_window; }
Renderer& Game::renderer() noexcept { return m_renderer; }
AudioManager& Game::audio() noexcept { return m_audio; }

float Game::delta_time() const noexcept { return m_delta_time; }

void Game::quit() noexcept { m_running = false; }

} // namespace core