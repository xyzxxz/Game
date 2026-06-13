#pragma once
#include <SDL3/SDL.h>
#include "board.h"
#include "renderer.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    SDL_Window* win_ = nullptr;
    Renderer renderer_;
    Board board_;
    int selRow_ = -1, selCol_ = -1;
    bool running_ = true;

    void handleEvents();
    void handleKey(SDL_Keycode key);
    void handleClick(float x, float y);
};