#include "planet.hpp"
#include "core/window.hpp"

namespace game {

Planet::Planet(float x, float y)
    : x_{x}
    , y_{y}
{
}

void Planet::update(float dt)
{
    if (!alive_) return;
    y_ += speed_ * dt;
}

void Planet::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (!alive_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

float Planet::x() const noexcept { return x_; }
float Planet::y() const noexcept { return y_; }

bool Planet::hit()
{
    if (!alive_) return false;
    hp_--;
    if (hp_ <= 0) {
        alive_ = false;
        return true;  // 被摧毁
    }
    return false;
}

int Planet::hp() const noexcept { return hp_; }
bool Planet::is_alive() const noexcept { return alive_; }

SDL_FRect Planet::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game