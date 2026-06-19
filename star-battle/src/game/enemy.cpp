#include "enemy.hpp"
#include "core/window.hpp"

namespace game {

Enemy::Enemy(float x, float y)
    : x_{x}
    , y_{y}
{
}

void Enemy::update(float dt)
{
    if (!alive_) return;
    y_ += speed_ * dt;
}

void Enemy::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (!alive_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

float Enemy::x() const noexcept { return x_; }
float Enemy::y() const noexcept { return y_; }

bool Enemy::is_alive() const noexcept { return alive_; }

void Enemy::destroy() noexcept { alive_ = false; }

SDL_FRect Enemy::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game