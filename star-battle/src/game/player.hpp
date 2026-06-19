#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace game {

/// @brief 玩家飞船
/// - WASD 移动
/// - Space 发射子弹
/// - 燃料初始 15，每秒 -1，归零游戏结束
/// - 最大燃料 30
class Player {
public:
    Player();

    void update(float dt);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);

    void move(float dx, float dy);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;
    [[nodiscard]] float speed() const noexcept;

    [[nodiscard]] int fuel() const noexcept;
    void add_fuel(int amount);
    void deduct_fuel(int amount);

    [[nodiscard]] bool is_alive() const noexcept;

    [[nodiscard]] SDL_FRect rect() const noexcept;

    static constexpr float kWidth = 60.0f;
    static constexpr float kHeight = 60.0f;
    static constexpr int kMaxFuel = 30;

private:
    float x_{0.0f};
    float y_{0.0f};
    float speed_{300.0f};   // 像素/秒
    int fuel_{15};           // 燃料值
    bool alive_{true};
};

} // namespace game