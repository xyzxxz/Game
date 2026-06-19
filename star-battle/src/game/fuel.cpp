#include "fuel.hpp"
#include "core/window.hpp"

namespace game {

Fuel::Fuel(float x, float y)
    : x_{x}
    , y_{y}
{
}

void Fuel::update(float dt)
{
    if (collected_) return;
    y_ += speed_ * dt;
}

void Fuel::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    if (collected_ || !texture) return;

    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

float Fuel::x() const noexcept { return x_; }
float Fuel::y() const noexcept { return y_; }

bool Fuel::is_off_screen(float screen_h) const noexcept
{
    return y_ > screen_h + kHeight;
}

bool Fuel::is_collected() const noexcept { return collected_; }

void Fuel::collect() noexcept { collected_ = true; }

SDL_FRect Fuel::rect() const noexcept
{
    return {x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
}

} // namespace game