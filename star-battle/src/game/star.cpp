#include "star.hpp"
#include "core/window.hpp"
#include <random>

namespace game {

Star::Star(float x, float y, float speed)
    : x_{x}
    , y_{y}
    , speed_{speed}
{
}

void Star::update(float dt)
{
    y_ += speed_ * dt;
}

void Star::render(SDL_Renderer* renderer)
{
    SDL_FRect dst{x_ - kWidth / 2.0f, y_ - kHeight / 2.0f, kWidth, kHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
    SDL_RenderFillRect(renderer, &dst);
}

float Star::x() const noexcept { return x_; }
float Star::y() const noexcept { return y_; }

bool Star::is_off_screen(float screen_h) const noexcept
{
    return y_ > screen_h + kHeight;
}

StarManager::StarManager(float screen_w, float screen_h)
    : screen_w_{screen_w}
    , screen_h_{screen_h}
    , rng_{std::random_device{}()}
{
}

void StarManager::update(float dt)
{
    spawn_timer_ += dt;
    while (spawn_timer_ >= kSpawnInterval) {
        spawn_timer_ -= kSpawnInterval;
        spawn_star();
    }

    for (auto& star : stars_) {
        star.update(dt);
    }

    std::erase_if(stars_, [this](const Star& s) {
        return s.is_off_screen(screen_h_);
    });
}

void StarManager::render(SDL_Renderer* renderer)
{
    for (auto& star : stars_) {
        star.render(renderer);
    }
}

void StarManager::spawn_star()
{
    std::uniform_real_distribution<float> x_dist(0.0f, screen_w_);
    std::uniform_real_distribution<float> speed_dist(20.0f, 60.0f);

    stars_.emplace_back(x_dist(rng_), -Star::kHeight, speed_dist(rng_));
}

} // namespace game