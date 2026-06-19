#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace ui {

/// @brief 游戏内 HUD（显示分数、燃料、暂停/静音状态）
class Hud {
public:
    Hud();

    void update(int score, int fuel, bool paused, bool muted);
    void render(SDL_Renderer* renderer);

    void set_score(int score) noexcept;
    void set_fuel(int fuel) noexcept;
    void set_paused(bool paused) noexcept;
    void set_muted(bool muted) noexcept;

private:
    int score_{0};
    int fuel_{15};
    bool paused_{false};
    bool muted_{false};
};

} // namespace ui