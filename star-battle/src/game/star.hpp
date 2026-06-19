#pragma once

#include <SDL3/SDL.h>
#include <cstdint>
#include <random>

namespace game {

/// @brief 背景星星（纯装饰）
class Star {
public:
    Star(float x, float y, float speed);

    void update(float dt);
    void render(SDL_Renderer* renderer);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;
    [[nodiscard]] bool is_off_screen(float screen_h) const noexcept;

    static constexpr float kWidth = 4.0f;
    static constexpr float kHeight = 4.0f;

private:
    float x_{0.0f};
    float y_{0.0f};
    float speed_{30.0f};
};

/// @brief 背景星星管理器
class StarManager {
public:
    StarManager() = default;
    explicit StarManager(float screen_w, float screen_h);

    void update(float dt);
    void render(SDL_Renderer* renderer);

private:
    void spawn_star();

    std::vector<Star> stars_;
    float screen_w_{};
    float screen_h_{};
    std::mt19937 rng_;
    float spawn_timer_{0.0f};
    static constexpr float kSpawnInterval = 0.3f;
};

} // namespace game