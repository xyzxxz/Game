#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Board;

class Renderer {
public:
    static constexpr int kCellSize = 60;

    Renderer(SDL_Window* window);
    ~Renderer();

    void draw(const Board& board, int selRow, int selCol);

private:
    SDL_Renderer* ren_ = nullptr;
    TTF_Font* font_ = nullptr;

    SDL_Texture* makeTexture(const char* text, SDL_Color color);
};