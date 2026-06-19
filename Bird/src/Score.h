#pragma once

#include <SDL3/SDL.h>

class Score {
public:
    Score();
    ~Score();

    bool Init(SDL_Renderer* renderer);
    void Reset();
    void SetScore(int score);
    int  GetScore() const;
    int  GetBestScore() const;
    int  GetMedalLevel() const;

    void Render(SDL_Renderer* renderer);
    void RenderSmall(SDL_Renderer* renderer, float x, float y);
    void RenderBest(SDL_Renderer* renderer, float x, float y);
    void RenderMedal(SDL_Renderer* renderer, float x, float y);
    void RenderNew(SDL_Renderer* renderer, float x, float y);

private:
    int          m_score;
    int          m_bestScore;
    SDL_Texture* m_digitTextures[10];
    SDL_Texture* m_digitTexturesSmall[10];
    SDL_Texture* m_medalTextures[4];
    SDL_Texture* m_newTexture;
};