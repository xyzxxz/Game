#pragma once

#include <SDL3/SDL.h>
#include "Button.h"

class Score;

class UI {
public:
    UI();
    ~UI();

    bool Init(SDL_Renderer* renderer);

    void RenderMenu(SDL_Renderer* renderer);
    void RenderHUD(SDL_Renderer* renderer, Score* score);
    void RenderGameOver(SDL_Renderer* renderer, Score* score);

    bool IsStartButtonClicked(float x, float y) const;
    bool IsOkButtonClicked(float x, float y) const;
    bool IsPauseButtonClicked(float x, float y) const;
    bool IsShareButtonClicked(float x, float y) const;

private:
    Button m_startButton;
    Button m_okButton;
    Button m_pauseButton;
    Button m_resumeButton;
    Button m_shareButton;
    Button m_scoreButton;

    SDL_Texture* m_logoTexture;
    SDL_Texture* m_messageTexture;
    SDL_Texture* m_gameOverTexture;
    SDL_Texture* m_panelTexture;
    SDL_Texture* m_pauseTexture;
    SDL_Texture* m_resumeTexture;
    SDL_Texture* m_scoreTexture;
    SDL_Texture* m_shareTexture;
};