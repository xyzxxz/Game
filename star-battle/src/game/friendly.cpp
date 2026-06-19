#include "friendly.hpp"
#include "core/window.hpp"

namespace game {

Friendly::Friendly(float x, float y)
    : x_{x}
    , y_{y}
{
}

void Friendly::update(float dt)
{
    if (!alive_) return;
    y_ += speed_ * dt;
}

void Friendly::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (!alive_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

float Friendly::x() const noexcept { return x_; }
float Friendly::y() const noexcept { return y_; }

bool Friendly::is_alive() const noexcept { return alive_; }

void Friendly::destroy() noexcept { alive_ = false; }

SDL_FRect Friendly::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game