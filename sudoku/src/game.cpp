#include "game.h"

Game::Game()
    : win_(SDL_CreateWindow("Sudoku", 540, 540, 0)),
      renderer_(win_) {
}

Game::~Game() {
    SDL_DestroyWindow(win_);
    TTF_Quit();
}

void Game::run() {
    while (running_) {
        handleEvents();
        renderer_.draw(board_, selRow_, selCol_);
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT:
            running_ = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            handleKey(e.key.key);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            handleClick(e.button.x, e.button.y);
            break;
        }
    }
}

void Game::handleKey(SDL_Keycode key) {
    if (selRow_ < 0 || board_.isFixed(selRow_, selCol_)) return;

    if (key >= SDLK_1 && key <= SDLK_9) {
        board_.set(selRow_, selCol_, key - SDLK_1 + 1);
    } else if (key == SDLK_BACKSPACE || key == SDLK_DELETE) {
        board_.set(selRow_, selCol_, 0);
    }

    if (board_.isComplete()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Sudoku", "Congratulations! You solved it!", win_);
        running_ = false;
    }
}

void Game::handleClick(float x, float y) {
    int col = int(x) / Renderer::kCellSize;
    int row = int(y) / Renderer::kCellSize;
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        selRow_ = row;
        selCol_ = col;
    }
}