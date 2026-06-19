#include "Background.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"

Background::Background()
    : m_scrollX(0.0f)
    , m_texture(nullptr) {
}

Background::~Background() {
    if (m_texture) SDL_DestroyTexture(m_texture);
}

bool Background::Init(SDL_Renderer* renderer) {
    m_texture = IMG_LoadTexture(renderer, "assets/sprites/background-day.png");
    return m_texture != nullptr;
}

void Background::Update(float dt) {
    m_scrollX -= kGroundSpeed * 0.5f * dt;
    // 背景纹理宽度为288，正好等于屏幕宽度，无需循环
    if (m_scrollX <= -kScreenWidth) {
        m_scrollX += kScreenWidth;
    }
}

void Background::Render(SDL_Renderer* renderer) {
    SDL_FRect dst = {m_scrollX, 0, kScreenWidth, kScreenHeight};
    SDL_RenderTexture(renderer, m_texture, nullptr, &dst);

    // 循环渲染第二份
    dst.x = m_scrollX + kScreenWidth;
    SDL_RenderTexture(renderer, m_texture, nullptr, &dst);
}