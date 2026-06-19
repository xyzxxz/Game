#include "Button.h"

Button::Button()
    : m_texture(nullptr) {
    m_rect = {0, 0, 0, 0};
}

Button::~Button() {
    // 不负责释放纹理，纹理由 UI 管理
}

void Button::SetRect(float x, float y, float w, float h) {
    m_rect = {x, y, w, h};
}

void Button::SetTexture(SDL_Texture* texture) {
    m_texture = texture;
}

bool Button::IsClicked(float mouseX, float mouseY) const {
    return mouseX >= m_rect.x && mouseX <= m_rect.x + m_rect.w &&
           mouseY >= m_rect.y && mouseY <= m_rect.y + m_rect.h;
}

void Button::Render(SDL_Renderer* renderer) {
    if (m_texture) {
        SDL_RenderTexture(renderer, m_texture, nullptr, &m_rect);
    }
}