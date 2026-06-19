#include "Score.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"
#include <cstdio>

Score::Score()
    : m_score(0)
    , m_bestScore(0)
    , m_newTexture(nullptr) {
    for (auto& tex : m_digitTextures) tex = nullptr;
    for (auto& tex : m_digitTexturesSmall) tex = nullptr;
    for (auto& tex : m_medalTextures) tex = nullptr;
}

Score::~Score() {
    for (auto& tex : m_digitTextures) if (tex) SDL_DestroyTexture(tex);
    for (auto& tex : m_digitTexturesSmall) if (tex) SDL_DestroyTexture(tex);
    for (auto& tex : m_medalTextures) if (tex) SDL_DestroyTexture(tex);
    if (m_newTexture) SDL_DestroyTexture(m_newTexture);
}

bool Score::Init(SDL_Renderer* renderer) {
    // 大号数字 0-9
    char path[64];
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "assets/sprites/%d.png", i);
        m_digitTextures[i] = IMG_LoadTexture(renderer, path);
        if (!m_digitTextures[i]) return false;
    }

    // 小号数字 0-9
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "assets/sprites/%d_small.png", i);
        m_digitTexturesSmall[i] = IMG_LoadTexture(renderer, path);
        if (!m_digitTexturesSmall[i]) return false;
    }

    // 奖牌
    m_medalTextures[0] = IMG_LoadTexture(renderer, "assets/sprites/bronze-medal.png");
    m_medalTextures[1] = IMG_LoadTexture(renderer, "assets/sprites/silver-medal.png");
    m_medalTextures[2] = IMG_LoadTexture(renderer, "assets/sprites/gold-medal.png");
    m_medalTextures[3] = IMG_LoadTexture(renderer, "assets/sprites/platinum-medal.png");

    m_newTexture = IMG_LoadTexture(renderer, "assets/sprites/new.png");

    return true;
}

void Score::Reset() {
    m_score = 0;
}

void Score::SetScore(int score) {
    m_score = score;
    if (m_score > m_bestScore) {
        m_bestScore = m_score;
    }
}

int Score::GetScore() const {
    return m_score;
}

int Score::GetBestScore() const {
    return m_bestScore;
}

int Score::GetMedalLevel() const {
    if (m_score >= 40) return 3;  // platinum
    if (m_score >= 30) return 2;  // gold
    if (m_score >= 20) return 1;  // silver
    if (m_score >= 10) return 0;  // bronze
    return -1;                     // no medal
}

void Score::Render(SDL_Renderer* renderer) {
    // 在屏幕顶部居中绘制大号分数
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", m_score);
    int len = static_cast<int>(strlen(buf));

    constexpr float digitW = 24.0f;
    constexpr float digitH = 36.0f;
    float totalW = digitW * len;
    float startX = (kScreenWidth - totalW) / 2.0f;

    for (int i = 0; i < len; i++) {
        int digit = buf[i] - '0';
        SDL_FRect dst = {startX + i * digitW, 30.0f, digitW, digitH};
        SDL_RenderTexture(renderer, m_digitTextures[digit], nullptr, &dst);
    }
}

void Score::RenderSmall(SDL_Renderer* renderer, float x, float y) {
    // 小号数字渲染
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", m_score);
    int len = static_cast<int>(strlen(buf));

    constexpr float digitW = 16.0f;
    constexpr float digitH = 20.0f;
    float totalW = digitW * len;

    for (int i = 0; i < len; i++) {
        int digit = buf[i] - '0';
        SDL_FRect dst = {x + i * digitW, y, digitW, digitH};
        SDL_RenderTexture(renderer, m_digitTexturesSmall[digit], nullptr, &dst);
    }
}

void Score::RenderBest(SDL_Renderer* renderer, float x, float y) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", m_bestScore);
    int len = static_cast<int>(strlen(buf));

    constexpr float digitW = 16.0f;
    constexpr float digitH = 20.0f;
    float totalW = digitW * len;

    for (int i = 0; i < len; i++) {
        int digit = buf[i] - '0';
        SDL_FRect dst = {x + i * digitW, y, digitW, digitH};
        SDL_RenderTexture(renderer, m_digitTexturesSmall[digit], nullptr, &dst);
    }
}

void Score::RenderMedal(SDL_Renderer* renderer, float x, float y) {
    int level = GetMedalLevel();
    if (level < 0) return;

    constexpr float medalSize = 44.0f;
    SDL_FRect dst = {x, y, medalSize, medalSize};
    SDL_RenderTexture(renderer, m_medalTextures[level], nullptr, &dst);
}

void Score::RenderNew(SDL_Renderer* renderer, float x, float y) {
    if (m_newTexture) {
        SDL_FRect dst = {x, y, 32.0f, 14.0f};
        SDL_RenderTexture(renderer, m_newTexture, nullptr, &dst);
    }
}