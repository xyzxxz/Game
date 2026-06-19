#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace game {

/// @brief 敌方飞船
/// - 击中 +5 分
/// - 碰撞损失 15 点燃料
class Enemy {
public:
    Enemy(float x, float y);

    void update(float dt);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;

    [[nodiscard]] bool is_alive() const noexcept;
    void destroy() noexcept;

    [[nodiscard]] SDL_FRect rect() const noexcept;

    static constexpr float kWidth = 50.0f;
    static constexpr float kHeight = 40.0f;

private:
    float x_{0.0f};
    float y_{0.0f};
    float speed_{150.0f};
    bool alive_{true};
};

} // namespace game