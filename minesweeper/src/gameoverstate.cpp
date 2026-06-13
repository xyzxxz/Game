#include "gameoverstate.hpp"
#include "game.hpp"
#include "menustate.hpp"
#include "render_text.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>

GameOverState::GameOverState(Game& game, GameResult result)
    : GameState(game), m_result(result) {}

void GameOverState::enter() {}
void GameOverState::exit() {}

void GameOverState::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_RETURN || event.key.key == SDLK_SPACE) {
            m_game.changeState(std::make_unique<MenuState>(m_game));
        }
    }
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        m_game.changeState(std::make_unique<MenuState>(m_game));
    }
}

void GameOverState::update() {}

void GameOverState::render() {
    SDL_Renderer* renderer = m_game.renderer();
    TTF_Font* font = m_game.getFont();
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    
    int w, h;
    SDL_GetWindowSize(m_game.window(), &w, &h);
    
    SDL_Color winColor = { 0x27, 0xae, 0x60, 0xff };
    SDL_Color loseColor = { 0xe7, 0x4c, 0x3c, 0xff };
    SDL_Color textColor = { 0xec, 0xf0, 0xf1, 0xff };
    
    const char* resultText = (m_result == GameResult::Win) ? m_win.data() : m_lose.data();
    SDL_Color resultColor = (m_result == GameResult::Win) ? winColor : loseColor;
    
    renderTextCentered(renderer, font, resultText, resultColor, w / 2, h / 2 - 50);
    renderTextCentered(renderer, font, "按 Enter 或 Space 返回菜单", textColor, w / 2, h / 2 + 50);
    renderTextCentered(renderer, font, "或点击鼠标任意位置", textColor, w / 2, h / 2 + 100);
    
    SDL_RenderPresent(renderer);
}
