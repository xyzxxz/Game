#pragma once

#include <SDL3/SDL.h>
#include "model/types.hpp"
#include "model/score.hpp"
#include "resource.hpp"
#include "input.hpp"
#include "view/menu_renderer.hpp"
#include "view/level_select_renderer.hpp"
#include "view/stage_renderer.hpp"
#include "view/hud_renderer.hpp"
#include "logic/collision.hpp"
#include "logic/ghost_ai.hpp"
#include "logic/stage_controller.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int           m_fps      = 120;
    bool          m_running  = true;

    GameState m_state          = GameState::Menu;
    int       m_unlockedLevel  = 1;

    ResourceManager  m_resource;
    InputManager     m_input;
    MenuRenderer     m_menuRenderer;
    LevelSelectRenderer  m_levelSelectRenderer;
    StageRenderer         m_stageRenderer;
    HudRenderer           m_hudRenderer;
    GhostAI               m_ghostAI;
    Collision             m_collision;

    Score                m_score;
    StageController*     m_stageCtrl = nullptr;

    // 帧率控制
    float m_lastTick = 0.0f;
    float capFPS(float startTicks);

    // 各状态的 update
    void handleEvents();
    void updateMenu(float dt);
    void updateLevelSelect(float dt);
    void updatePlaying(float dt);
    void updatePaused(float dt);
    void updateLifeLost(float dt);
    void updateStageClear(float dt);
    void updateGameOver(float dt);
    void updateGameWin(float dt);

    // 关卡切换
    void startStage(int level);
    void destroyStage();
};
