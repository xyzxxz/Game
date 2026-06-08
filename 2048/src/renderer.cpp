module;

#include <SDL3/SDL.h>

module renderer;

namespace renderer {

// ---- BoardRenderer ----
BoardRenderer::BoardRenderer() {}

void BoardRenderer::render(SDL_Renderer* renderer, const game::Board& board,
                           int windowW, int windowH) {}

bool BoardRenderer::isAnimating() const { return false; }

void BoardRenderer::updateAnimation(float dt) {}

void BoardRenderer::drawTile(SDL_Renderer* renderer, int value,
                             int x, int y, int tileSize) {}

Color BoardRenderer::getTileColor(int value) const { return Colors::kTileDefault; }

Color BoardRenderer::getTextColor(int value) const { return Colors::kTextDark; }

// ---- MenuRenderer ----
MenuRenderer::MenuRenderer() {
    m_items[0] = { "Start" };
    m_items[1] = { "Quit"  };
}

void MenuRenderer::render(SDL_Renderer* renderer, int selectedIndex,
                          int windowW, int windowH) {}

// ---- HUDRenderer ----
HUDRenderer::HUDRenderer() {}

void HUDRenderer::render(SDL_Renderer* renderer, const game::Game& game,
                         int windowW, int windowH) {}

} // namespace renderer
