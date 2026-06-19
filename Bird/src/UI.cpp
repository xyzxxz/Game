#include "UI.h"
#include "Score.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"

UI::UI()
    : m_logoTexture(nullptr)
    , m_messageTexture(nullptr)
    , m_gameOverTexture(nullptr)
    , m_panelTexture(nullptr)
    , m_pauseTexture(nullptr)
    , m_resumeTexture(nullptr)
    , m_scoreTexture(nullptr)
    , m_shareTexture(nullptr) {
}

UI::~UI() {
    if (m_logoTexture)    SDL_DestroyTexture(m_logoTexture);
    if (m_messageTexture) SDL_DestroyTexture(m_messageTexture);
    if (m_gameOverTexture) SDL_DestroyTexture(m_gameOverTexture);
    if (m_panelTexture)   SDL_DestroyTexture(m_panelTexture);
    if (m_pauseTexture)   SDL_DestroyTexture(m_pauseTexture);
    if (m_resumeTexture)  SDL_DestroyTexture(m_resumeTexture);
    if (m_scoreTexture)   SDL_DestroyTexture(m_scoreTexture);
    if (m_shareTexture)   SDL_DestroyTexture(m_shareTexture);
}

bool UI::Init(SDL_Renderer* renderer) {
    m_logoTexture    = IMG_LoadTexture(renderer, "assets/sprites/logo.png");
    m_messageTexture = IMG_LoadTexture(renderer, "assets/sprites/message.png");
    m_gameOverTexture = IMG_LoadTexture(renderer, "assets/sprites/gameover.png");
    m_panelTexture   = IMG_LoadTexture(renderer, "assets/sprites/panel.png");

    m_pauseTexture   = IMG_LoadTexture(renderer, "assets/buttons/pause.png");
    m_resumeTexture  = IMG_LoadTexture(renderer, "assets/buttons/resume.png");
    m_scoreTexture   = IMG_LoadTexture(renderer, "assets/buttons/score.png");
    m_shareTexture   = IMG_LoadTexture(renderer, "assets/buttons/share.png");

    // 开始按钮：message.png 中的 play 按钮区域（下方）
    // message.png 尺寸约 184 x 267
    m_startButton.SetRect(
        (kScreenWidth - 104.0f) / 2.0f,
        kScreenHeight * 0.6f,
        104.0f, 58.0f);

    // OK 按钮：panel 下方
    m_okButton.SetRect(
        (kScreenWidth - 80.0f) / 2.0f,
        kScreenHeight - 120.0f,
        80.0f, 28.0f);

    // 暂停按钮：右上角
    m_pauseButton.SetRect(
        kScreenWidth - 34.0f,
        10.0f,
        26.0f, 28.0f);

    // 分数按钮（暂时不用）
    m_scoreButton.SetRect(0, 0, 0, 0);
    m_shareButton.SetRect(0, 0, 0, 0);

    return true;
}

void UI::RenderMenu(SDL_Renderer* renderer) {
    // Logo
    constexpr float logoW = 178.0f;
    constexpr float logoH = 48.0f;
    SDL_FRect logoDst = {
        (kScreenWidth - logoW) / 2.0f,
        80.0f,
        logoW, logoH
    };
    SDL_RenderTexture(renderer, m_logoTexture, nullptr, &logoDst);

    // Message (get ready) - 包含 play 按钮的外观
    constexpr float msgW = 184.0f;
    constexpr float msgH = 267.0f;
    SDL_FRect msgDst = {
        (kScreenWidth - msgW) / 2.0f,
        kScreenHeight * 0.28f,
        msgW, msgH
    };
    SDL_RenderTexture(renderer, m_messageTexture, nullptr, &msgDst);
}

void UI::RenderHUD(SDL_Renderer* renderer, Score* score) {
    score->Render(renderer);
}

void UI::RenderGameOver(SDL_Renderer* renderer, Score* score) {
    // Game Over 标题
    constexpr float goW = 192.0f;
    constexpr float goH = 42.0f;
    SDL_FRect goDst = {
        (kScreenWidth - goW) / 2.0f,
        60.0f,
        goW, goH
    };
    SDL_RenderTexture(renderer, m_gameOverTexture, nullptr, &goDst);

    // 面板
    constexpr float panelW = 226.0f;
    constexpr float panelH = 128.0f;
    float panelX = (kScreenWidth - panelW) / 2.0f;
    float panelY = 120.0f;
    SDL_FRect panelDst = {panelX, panelY, panelW, panelH};
    SDL_RenderTexture(renderer, m_panelTexture, nullptr, &panelDst);

    // 奖牌
    score->RenderMedal(renderer, panelX + 30.0f, panelY + 44.0f);

    // 当前分数（面板内右侧）
    score->RenderSmall(renderer, panelX + 170.0f, panelY + 34.0f);

    // 最高分（面板内右侧下方）
    score->RenderBest(renderer, panelX + 170.0f, panelY + 74.0f);

    // NEW 标签
    if (score->GetScore() > 0 && score->GetScore() >= score->GetBestScore()) {
        score->RenderNew(renderer, panelX + 150.0f, panelY + 55.0f);
    }

    // OK 按钮
    m_okButton.Render(renderer);
}

// ===== 按钮点击检测 =====

bool UI::IsStartButtonClicked(float x, float y) const {
    return m_startButton.IsClicked(x, y);
}

bool UI::IsOkButtonClicked(float x, float y) const {
    return m_okButton.IsClicked(x, y);
}

bool UI::IsPauseButtonClicked(float x, float y) const {
    return m_pauseButton.IsClicked(x, y);
}

bool UI::IsShareButtonClicked(float x, float y) const {
    return m_shareButton.IsClicked(x, y);
}