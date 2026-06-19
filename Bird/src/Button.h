#pragma once

#include <SDL3/SDL.h>

class Button {
public:
    Button();
    ~Button();

    void SetRect(float x, float y, float w, float h);
    void SetTexture(SDL_Texture* texture);

    bool IsClicked(float mouseX, float mouseY) const;
    void Render(SDL_Renderer* renderer);

private:
    SDL_FRect    m_rect;
    SDL_Texture* m_texture;
};