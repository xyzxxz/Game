#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace game {

/// @brief 子弹类型
enum class BulletType : Uint8 {
    kPlayer,  // 玩家子弹
    kEnemy,   // 敌方子弹
};

/// @brief 子弹实体
class Bullet {
public:
    Bullet(float x, float y, BulletType type, float dx = 0.0f, float dy = 0.0f);

    void update(float dt);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;
    [[nodiscard]] BulletType type() const noexcept;

    [[nodiscard]] bool is_off_screen(float screen_w, float screen_h) const noexcept;

    [[nodiscard]] bool is_active() const noexcept;
    void deactivate() noexcept;

    [[nodiscard]] SDL_FRect rect() const noexcept;

    static constexpr float kWidth = 8.0f;
    static constexpr float kHeight = 20.0f;

private:
    float x_{0.0f};
    float y_{0.0f};
    float dx_{0.0f};
    float dy_{0.0f};
    float speed_{500.0f};
    BulletType type_{BulletType::kPlayer};
    bool active_{true};
};

} // namespace game