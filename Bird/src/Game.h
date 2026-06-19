#pragma once

#include <SDL3/SDL.h>
#include "Config.h"
#include "Bird.h"
#include "Pipe.h"
#include "Ground.h"
#include "Background.h"
#include "Score.h"
#include "UI.h"
#include "Audio.h"

class Game {
public:
    Game();
    ~Game();

    bool Init(SDL_Window* window, SDL_Renderer* renderer);
    void HandleEvent(const SDL_Event* event);
    void Update(float dt);
    void Render(SDL_Renderer* renderer);
    void Quit();

    bool ShouldQuit() const;

private:
    GameState m_state;
    bool      m_quit;
    bool      m_paused;

    Bird         m_bird;
    PipeManager  m_pipes;
    Ground       m_ground;
    Background   m_background;
    Score        m_score;
    UI           m_ui;
    Audio        m_audio;

    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;

    void HandleMenuEvent(const SDL_Event* event);
    void HandlePlayingEvent(const SDL_Event* event);
    void HandleGameOverEvent(const SDL_Event* event);

    void UpdatePlaying(float dt);

    void StartGame();
    void OnGameOver();
    void RestartGame();
};