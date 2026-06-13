#include "menustate.hpp"
#include "game.hpp"
#include "playingstate.hpp"
#include "render_text.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>

MenuState::MenuState(Game& game) : GameState(game) {}

void MenuState::enter() {}
void MenuState::exit() {}

void MenuState::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_1:
                m_selected = Difficulty::Easy;
                break;
            case SDLK_2:
                m_selected = Difficulty::Normal;
                break;
            case SDLK_3:
                m_selected = Difficulty::Hard;
                break;
            case SDLK_RETURN:
                m_game.changeState(std::make_unique<PlayingState>(m_game, m_selected));
                break;
            default:
                break;
        }
    }
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        
        int w, h;
        SDL_GetWindowSize(m_game.window(), &w, &h);
        
        int menuY = h / 2;
        
        int itemHeight = 40;
        int itemWidth = 200;
        
        SDL_FRect easyRect = { static_cast<float>(w/2 - itemWidth/2), static_cast<float>(menuY - itemHeight/2), 
                               static_cast<float>(itemWidth), static_cast<float>(itemHeight) };
        SDL_FRect normalRect = { static_cast<float>(w/2 - itemWidth/2), static_cast<float>(menuY + 40 - itemHeight/2), 
                                 static_cast<float>(itemWidth), static_cast<float>(itemHeight) };
        SDL_FRect hardRect = { static_cast<float>(w/2 - itemWidth/2), static_cast<float>(menuY + 80 - itemHeight/2), 
                               static_cast<float>(itemWidth), static_cast<float>(itemHeight) };
        SDL_FRect startRect = { static_cast<float>(w/2 - itemWidth/2), static_cast<float>(h/2 + 120 - itemHeight/2), 
                                static_cast<float>(itemWidth), static_cast<float>(itemHeight) };
        
        if (x >= easyRect.x && x <= easyRect.x + easyRect.w &&
            y >= easyRect.y && y <= easyRect.y + easyRect.h) {
            m_selected = Difficulty::Easy;
        } else if (x >= normalRect.x && x <= normalRect.x + normalRect.w &&
                   y >= normalRect.y && y <= normalRect.y + normalRect.h) {
            m_selected = Difficulty::Normal;
        } else if (x >= hardRect.x && x <= hardRect.x + hardRect.w &&
                   y >= hardRect.y && y <= hardRect.y + hardRect.h) {
            m_selected = Difficulty::Hard;
        } else if (x >= startRect.x && x <= startRect.x + startRect.w &&
                   y >= startRect.y && y <= startRect.y + startRect.h) {
            m_game.changeState(std::make_unique<PlayingState>(m_game, m_selected));
        }
    }
}

void MenuState::update() {}

void MenuState::render() {
    SDL_Renderer* renderer = m_game.renderer();
    TTF_Font* font = m_game.getFont();
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    
    int w, h;
    SDL_GetWindowSize(m_game.window(), &w, &h);
    
    SDL_Color titleColor = { 0x34, 0x98, 0xdb, 0xff };
    SDL_Color textColor = { 0xec, 0xf0, 0xf1, 0xff };
    SDL_Color highlightColor = { 0xe6, 0x7e, 0x22, 0xff };
    
    renderTextCentered(renderer, font, m_title.data(), titleColor, w / 2, h / 2 - 150);
    
    renderTextCentered(renderer, font, m_difficulty.data(), textColor, w / 2, h / 2 - 60);
    
    int menuY = h / 2;
    SDL_Color color1 = (m_selected == Difficulty::Easy) ? highlightColor : textColor;
    SDL_Color color2 = (m_selected == Difficulty::Normal) ? highlightColor : textColor;
    SDL_Color color3 = (m_selected == Difficulty::Hard) ? highlightColor : textColor;
    
    renderTextCentered(renderer, font, menu1.data(), color1, w / 2, menuY);
    renderTextCentered(renderer, font, menu2.data(), color2, w / 2, menuY + 40);
    renderTextCentered(renderer, font, menu3.data(), color3, w / 2, menuY + 80);
    
    renderTextCentered(renderer, font, m_start.data(), textColor, w / 2, h / 2 + 120);
    
    SDL_RenderPresent(renderer);
}

Difficulty MenuState::selectedDifficulty() const { return m_selected; }
