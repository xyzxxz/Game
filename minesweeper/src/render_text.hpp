#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstring>

inline SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, 
                               const char* text, const SDL_Color& color) {
    size_t len = std::strlen(text);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, len, color);
    if (!surface) {
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

inline void renderTextCentered(SDL_Renderer* renderer, TTF_Font* font,
                               const char* text, const SDL_Color& color,
                               int x, int y) {
    size_t len = std::strlen(text);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, len, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }
    
    int w = surface->w;
    int h = surface->h;
    
    SDL_FRect dstRect = { static_cast<float>(x - w / 2), static_cast<float>(y - h / 2), 
                           static_cast<float>(w), static_cast<float>(h) };
    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
    
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

inline void renderTextLeft(SDL_Renderer* renderer, TTF_Font* font,
                           const char* text, const SDL_Color& color,
                           int x, int y) {
    size_t len = std::strlen(text);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, len, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }
    
    int w = surface->w;
    int h = surface->h;
    
    SDL_FRect dstRect = { static_cast<float>(x), static_cast<float>(y), 
                           static_cast<float>(w), static_cast<float>(h) };
    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
    
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}
