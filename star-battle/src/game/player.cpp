#include "player.hpp"
#include "core/window.hpp"
#include <algorithm>

namespace game {

Player::Player()
    : x_{core::kDefaultWindowWidth / 2.0f}
    , y_{core::kDefaultWindowHeight - 80.0f}
{
}

void Player::update(float dt)
{
    if (!alive_) return;

    // 燃料每秒递减 1
    fuel_ = std::max(0, fuel_ - static_cast<int>(dt));
    if (fuel_ <= 0) {
        alive_ = false;
    }
}

void Player::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (!alive_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

void Player::move(float dx, float dy)
{
    if (!alive_) return;

    x_ += dx * speed_;
    y_ += dy * speed_;

    x_ = std::clamp(x_, kWidth / 2.0f, core::kDefaultWindowWidth - kWidth / 2.0f);
    y_ = std::clamp(y_, kHeight / 2.0f, core::kDefaultWindowHeight - kHeight / 2.0f);
}

float Player::x() const noexcept { return x_; }
float Player::y() const noexcept { return y_; }
float Player::speed() const noexcept { return speed_; }

int Player::fuel() const noexcept { return fuel_; }

void Player::add_fuel(int amount)
{
    fuel_ = std::min(fuel_ + amount, kMaxFuel);
}

void Player::deduct_fuel(int amount)
{
    fuel_ -= amount;
    if (fuel_ <= 0) {
        fuel_ = 0;
        alive_ = false;
    }
}

bool Player::is_alive() const noexcept { return alive_; }

SDL_FRect Player::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game