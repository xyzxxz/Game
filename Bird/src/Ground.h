#pragma once

#include <SDL3/SDL.h>

class Ground {
public:
    Ground();
    ~Ground();

    bool Init(SDL_Renderer* renderer);
    void Reset();
    void Update(float dt);
    void Render(SDL_Renderer* renderer);

    float GetY() const;

private:
    float m_scrollX;
    float m_y;
    SDL_Texture* m_texture;
};