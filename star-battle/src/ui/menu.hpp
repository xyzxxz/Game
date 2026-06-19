#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <string_view>

namespace ui {

/// @brief 游戏菜单（开始界面、结束界面）
class Menu {
public:
    enum class State {
        kStart,      // 开始界面
        kGameOver,   // 游戏结束
    };

    Menu();

    void set_state(State state) noexcept;
    [[nodiscard]] State state() const noexcept;

    void update(float dt);
    void render(SDL_Renderer* renderer);

    /// @brief 处理菜单事件，返回 true 表示事件被菜单消费
    bool handle_event(const SDL_Event& event);

    /// @brief 注册开始游戏回调
    void on_start_game(std::function<void()> callback);
    /// @brief 注册退出回调
    void on_quit(std::function<void()> callback);

    void set_final_score(int score) noexcept;

private:
    State state_{State::kStart};
    int final_score_{0};
    std::function<void()> start_callback_;
    std::function<void()> quit_callback_;
};

} // namespace ui