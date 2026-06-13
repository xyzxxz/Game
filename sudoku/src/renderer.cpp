#include "renderer.h"
#include "board.h"

Renderer::Renderer(SDL_Window* window) {
    ren_ = SDL_CreateRenderer(window, nullptr);
    TTF_Init();

    font_ = TTF_OpenFont("font.ttf", 36);
    if (!font_) SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
}

Renderer::~Renderer() {
    TTF_CloseFont(font_);
    SDL_DestroyRenderer(ren_);
}

void Renderer::draw(const Board& board, int selRow, int selCol) {
    // Black background
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, 255);
    SDL_RenderClear(ren_);

    const int total = kCellSize * 9;

    // Thin grid lines
    SDL_SetRenderDrawColor(ren_, 60, 60, 60, 255);
    for (int i = 1; i < 9; ++i) {
        if (i % 3 == 0) continue;
        SDL_FRect h{0, float(i * kCellSize), float(total), 1};
        SDL_FRect v{float(i * kCellSize), 0, 1, float(total)};
        SDL_RenderFillRect(ren_, &h);
        SDL_RenderFillRect(ren_, &v);
    }

    // Thick 3x3 box lines
    SDL_SetRenderDrawColor(ren_, 150, 150, 150, 255);
    for (int i = 3; i < 9; i += 3) {
        SDL_FRect h{0, float(i * kCellSize - 1), float(total), 3};
        SDL_FRect v{float(i * kCellSize - 1), 0, 3, float(total)};
        SDL_RenderFillRect(ren_, &h);
        SDL_RenderFillRect(ren_, &v);
    }

    // Draw numbers
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int val = board.get(r, c);
            if (val == 0) continue;

            SDL_Color color;
            if (board.isFixed(r, c)) {
                color = {200, 200, 200, 255};
            } else if (board.hasConflict(r, c)) {
                color = {220, 60, 60, 255};
            } else {
                color = {80, 180, 255, 255};
            }

            char buf[2] = {char('0' + val), '\0'};
            SDL_Texture* tex = makeTexture(buf, color);
            if (!tex) continue;

            float tw, th;
            SDL_GetTextureSize(tex, &tw, &th);
            SDL_FRect dst{
                float(c * kCellSize + (kCellSize - tw) / 2),
                float(r * kCellSize + (kCellSize - th) / 2),
                tw, th
            };
            SDL_RenderTexture(ren_, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
        }
    }

    // Selection highlight
    if (selRow >= 0 && selCol >= 0) {
        SDL_FRect rect{
            float(selCol * kCellSize), float(selRow * kCellSize),
            float(kCellSize), float(kCellSize)
        };
        SDL_SetRenderDrawColor(ren_, 50, 80, 140, 60);
        SDL_RenderFillRect(ren_, &rect);
        SDL_SetRenderDrawColor(ren_, 100, 160, 255, 255);
        SDL_RenderRect(ren_, &rect);
    }

    SDL_RenderPresent(ren_);
}

SDL_Texture* Renderer::makeTexture(const char* text, SDL_Color color) {
    if (!font_) return nullptr;
    SDL_Surface* surf = TTF_RenderText_Blended(font_, text, 0, color);
    if (!surf) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren_, surf);
    SDL_DestroySurface(surf);
    return tex;
}