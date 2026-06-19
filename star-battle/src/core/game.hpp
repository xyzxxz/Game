#pragma once

#include "window.hpp"
#include "renderer.hpp"
#include "audio.hpp"
#include <chrono>
#include <string_view>

namespace core {

/// @brief 游戏主类 — 管理初始化、主循环、事件分发
class Game {
public:
    Game(std::string_view title, int width = kDefaultWindowWidth,
         int height = kDefaultWindowHeight);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    /// @brief 启动主循环，阻塞直到退出
    int run();

    [[nodiscard]] Window& window() noexcept;
    [[nodiscard]] Renderer& renderer() noexcept;
    [[nodiscard]] AudioManager& audio() noexcept;

    [[nodiscard]] float delta_time() const noexcept;

    void quit() noexcept;

protected:
    virtual void on_init() {}
    virtual void on_event([[maybe_unused]] const SDL_Event& event) {}
    virtual void on_update([[maybe_unused]] float dt) {}
    virtual void on_render() {}
    virtual void on_cleanup() {}

private:
    void process_events();

    Window m_window;
    Renderer m_renderer;
    AudioManager m_audio;

    bool m_running{true};

    std::chrono::high_resolution_clock::time_point m_last_time;
    float m_delta_time{0.0f};
};

} // namespace core