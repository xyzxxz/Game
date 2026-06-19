#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace game {

/// @brief 行星
/// - 需要击中两次才能摧毁
/// - 摧毁 +10 分
class Planet {
public:
    Planet(float x, float y);

    void update(float dt);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;

    /// @brief 击中一次，减少生命值。返回是否被摧毁
    bool hit();

    [[nodiscard]] int hp() const noexcept;
    [[nodiscard]] bool is_alive() const noexcept;

    [[nodiscard]] SDL_FRect rect() const noexcept;

    static constexpr float kWidth = 55.0f;
    static constexpr float kHeight = 55.0f;

private:
    float x_{0.0f};
    float y_{0.0f};
    float speed_{60.0f};
    int hp_{2};           // 需要击中 2 次
    bool alive_{true};
};

} // namespace game