#include "hud.hpp"

namespace ui {

Hud::Hud() = default;

void Hud::update(int score, int fuel, bool paused, bool muted) {
    score_ = score;
    fuel_ = fuel;
    paused_ = paused;
    muted_ = muted;
}

void Hud::render(SDL_Renderer* /*renderer*/) {
    // 实际渲染由 StarBattleGame::render_hud() 处理
}

void Hud::set_score(int score) noexcept { score_ = score; }
void Hud::set_fuel(int fuel) noexcept { fuel_ = fuel; }
void Hud::set_paused(bool paused) noexcept { paused_ = paused; }
void Hud::set_muted(bool muted) noexcept { muted_ = muted; }

} // namespace ui