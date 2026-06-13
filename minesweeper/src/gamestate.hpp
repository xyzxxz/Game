#pragma once

#include <SDL3/SDL.h>

class Game;

// 状态模式 —— 抽象状态基类
class GameState {
public:
    GameState(Game& game) : m_game(game) {}
    virtual ~GameState() = default;

    virtual void enter()  = 0;
    virtual void exit()   = 0;
    virtual void handleEvent(const SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void render() = 0;

protected:
    Game& m_game;
};
