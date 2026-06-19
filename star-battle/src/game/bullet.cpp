#include "bullet.hpp"
#include "core/window.hpp"

namespace game {

Bullet::Bullet(float x, float y, BulletType type, float dx, float dy)
    : x_{x}
    , y_{y}
    , dx_{dx}
    , dy_{dy}
    , type_{type}
{
}

void Bullet::update(float dt)
{
    if (!active_) return;
    x_ += dx_ * speed_ * dt;
    y_ += dy_ * speed_ * dt;
}

void Bullet::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (!active_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

float Bullet::x() const noexcept { return x_; }
float Bullet::y() const noexcept { return y_; }
BulletType Bullet::type() const noexcept { return type_; }

bool Bullet::is_off_screen(float screen_w, float screen_h) const noexcept
{
    return x_ < -kWidth || x_ > screen_w + kWidth
        || y_ < -kHeight || y_ > screen_h + kHeight;
}

bool Bullet::is_active() const noexcept { return active_; }

void Bullet::deactivate() noexcept { active_ = false; }

SDL_FRect Bullet::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game