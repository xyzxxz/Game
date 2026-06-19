#include "Ground.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"

Ground::Ground()
    : m_scrollX(0.0f)
    , m_y(kGroundY)
    , m_texture(nullptr) {
}

Ground::~Ground() {
    if (m_texture) SDL_DestroyTexture(m_texture);
}

bool Ground::Init(SDL_Renderer* renderer) {
    m_texture = IMG_LoadTexture(renderer, "assets/sprites/base.png");
    return m_texture != nullptr;
}

void Ground::Reset() {
    m_scrollX = 0.0f;
}

void Ground::Update(float dt) {
    m_scrollX -= kGroundSpeed * dt;
    // base.png 宽度为336，需要循环
    if (m_scrollX <= -336.0f) {
        m_scrollX += 336.0f;
    }
}

void Ground::Render(SDL_Renderer* renderer) {
    SDL_FRect dst = {m_scrollX, m_y, 336.0f, kGroundHeight};
    SDL_RenderTexture(renderer, m_texture, nullptr, &dst);

    dst.x = m_scrollX + 336.0f;
    SDL_RenderTexture(renderer, m_texture, nullptr, &dst);
}

float Ground::GetY() const {
    return m_y;
}