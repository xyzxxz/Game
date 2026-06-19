#include "Bird.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"

Bird::Bird()
    : m_x(0.0f)
    , m_y(0.0f)
    , m_velocity(0.0f)
    , m_rotation(0.0f)
    , m_currentFrame(0)
    , m_animTimer(0.0f) {
    for (auto& tex : m_textures) {
        tex = nullptr;
    }
}

Bird::~Bird() {
    for (auto& tex : m_textures) {
        if (tex) SDL_DestroyTexture(tex);
    }
}

bool Bird::Init(SDL_Renderer* renderer) {
    m_textures[0] = IMG_LoadTexture(renderer, "assets/sprites/yellowbird-downflap.png");
    m_textures[1] = IMG_LoadTexture(renderer, "assets/sprites/yellowbird-midflap.png");
    m_textures[2] = IMG_LoadTexture(renderer, "assets/sprites/yellowbird-upflap.png");

    for (auto& tex : m_textures) {
        if (!tex) return false;
    }

    Reset();
    return true;
}

void Bird::Reset() {
    m_x = kScreenWidth * 0.3f - GetWidth() / 2.0f;
    m_y = kScreenHeight / 2.0f - GetHeight() / 2.0f;
    m_velocity = 0.0f;
    m_rotation = 0.0f;
    m_currentFrame = 1;  // midflap
    m_animTimer = 0.0f;
}

void Bird::Flap() {
    m_velocity = kFlapVelocity;
}

void Bird::Update(float dt) {
    // 动画
    m_animTimer += dt;
    if (m_animTimer >= kBirdAnimInterval) {
        m_animTimer -= kBirdAnimInterval;
        m_currentFrame = (m_currentFrame + 1) % 3;
    }

    // 物理
    m_velocity += kGravity * dt;
    if (m_velocity > kMaxFallSpeed) {
        m_velocity = kMaxFallSpeed;
    }

    m_y += m_velocity * dt;

    // 旋转：上升时向上倾斜，下降时向下倾斜
    if (m_velocity < 0) {
        m_rotation = kBirdRotationMax * (m_velocity / kFlapVelocity);
        if (m_rotation < -kBirdRotationMax) m_rotation = -kBirdRotationMax;
    } else {
        float t = m_velocity / kMaxFallSpeed;
        m_rotation = kBirdRotationMax * t * t;
        if (m_rotation > 90.0f) m_rotation = 90.0f;
    }
}

void Bird::Render(SDL_Renderer* renderer) {
    SDL_FRect dst = {m_x, m_y, GetWidth(), GetHeight()};
    SDL_FPoint center = {GetWidth() / 2.0f, GetHeight() / 2.0f};
    SDL_RenderTextureRotated(renderer, m_textures[m_currentFrame],
                             nullptr, &dst, m_rotation, &center, SDL_FLIP_NONE);
}

float Bird::GetX() const { return m_x; }
float Bird::GetY() const { return m_y; }
float Bird::GetWidth() const { return 34.0f; }
float Bird::GetHeight() const { return 24.0f; }
float Bird::GetRotation() const { return m_rotation; }

bool Bird::IsColliding(float x, float y, float w, float h) const {
    // AABB 碰撞检测（简化，小鸟用较小内缩矩形）
    float birdLeft   = m_x + 4.0f;
    float birdRight  = m_x + GetWidth() - 4.0f;
    float birdTop    = m_y + 4.0f;
    float birdBottom = m_y + GetHeight() - 4.0f;

    return !(birdRight < x || birdLeft > x + w ||
             birdBottom < y || birdTop > y + h);
}