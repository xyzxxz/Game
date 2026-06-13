#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <memory>

#include <SDL3/SDL.h>

#include "gamestate.hpp"
#include "difficulty.hpp"

class GameState;  // 前向声明

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

    // 状态机切换
    void changeState(std::unique_ptr<GameState> newState);

    // SDL 资源访问
    SDL_Window*   window()   const { return m_window; }
    SDL_Renderer* renderer() const { return m_renderer; }
    TTF_Font*     getFont()  const { return font; }

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* font;
    bool          m_running  = false;

    std::unique_ptr<GameState> m_currentState;
};
