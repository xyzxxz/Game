#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace game {

/// @brief 燃料瓶（掉落的补给品）
/// - 触碰增加 15 点燃料
/// - 最大上限 30
class Fuel {
public:
    Fuel(float x, float y);

    void update(float dt);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;

    [[nodiscard]] bool is_off_screen(float screen_h) const noexcept;
    [[nodiscard]] bool is_collected() const noexcept;
    void collect() noexcept;

    [[nodiscard]] SDL_FRect rect() const noexcept;

    static constexpr int kFuelAmount = 15;
    static constexpr float kWidth = 20.0f;
    static constexpr float kHeight = 28.0f;

private:
    float x_{0.0f};
    float y_{0.0f};
    float speed_{80.0f};  // 下落速度
    bool collected_{false};
};

} // namespace game