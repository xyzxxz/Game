#pragma once

#include <SDL3/SDL.h>

class Background {
public:
    Background();
    ~Background();

    bool Init(SDL_Renderer* renderer);
    void Update(float dt);
    void Render(SDL_Renderer* renderer);

private:
    float m_scrollX;
    SDL_Texture* m_texture;
};