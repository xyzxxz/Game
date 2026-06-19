#pragma once

#include <SDL3/SDL.h>

class Bird {
public:
    Bird();
    ~Bird();

    bool Init(SDL_Renderer* renderer);
    void Reset();
    void Flap();
    void Update(float dt);
    void Render(SDL_Renderer* renderer);

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetRotation() const;

    bool IsColliding(float x, float y, float w, float h) const;

private:
    float m_x;
    float m_y;
    float m_velocity;
    float m_rotation;
    SDL_Texture* m_textures[3];
    int          m_currentFrame;
    float        m_animTimer;
};