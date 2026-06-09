#include "game.hpp"

Game::Game()
    : m_resource(nullptr) {
    // TODO: 初始化 SDL 窗口与渲染器
    // m_resource.loadAll();
}

Game::~Game() {
    destroyStage();
    // TODO: 销毁窗口与渲染器
}

void Game::run() {
    while (m_running) {
        float dt = capFPS(SDL_GetTicks() / 1000.0f);

        handleEvents();

        switch (m_state) {
        case GameState::Menu:
            updateMenu(dt);
            m_menuRenderer.render(m_renderer, m_resource);
            break;
        case GameState::LevelSelect:
            updateLevelSelect(dt);
            m_levelSelectRenderer.render(m_renderer, m_resource, m_unlockedLevel);
            break;
        case GameState::Playing:
            updatePlaying(dt);
            if (m_stageCtrl) {
                m_stageRenderer.render(m_renderer, m_resource,
                    m_stageCtrl->getStage(), m_stageCtrl->getPlayer(),
                    m_stageCtrl->getGhosts());
                m_hudRenderer.render(m_renderer, m_resource, m_score);
            }
            break;
        case GameState::Paused:
            updatePaused(dt);
            // TODO: 在 Playing 画面上叠加暂停遮罩
            break;
        case GameState::LifeLost:
            updateLifeLost(dt);
            break;
        case GameState::StageClear:
            updateStageClear(dt);
            break;
        case GameState::GameOver:
            updateGameOver(dt);
            break;
        case GameState::GameWin:
            updateGameWin(dt);
            break;
        }

        SDL_RenderPresent(m_renderer);
    }
}

float Game::capFPS(float currentTime) {
    float targetFrameTime = 1.0f / m_fps;
    float elapsed = currentTime - m_lastTick;
    if (elapsed < targetFrameTime) {
        SDL_Delay((Uint32)((targetFrameTime - elapsed) * 1000));
        currentTime = SDL_GetTicks() / 1000.0f;
        elapsed = currentTime - m_lastTick;
    }
    m_lastTick = currentTime;
    return (elapsed < targetFrameTime * 2.0f) ? elapsed : targetFrameTime;
}

void Game::handleEvents() {
    m_input.update();
    if (m_input.isQuitRequested()) {
        m_running = false;
    }
}

// ===== 各状态 update（骨架）=====

void Game::updateMenu(float dt) {
    // TODO: 检测确认键 → m_state = LevelSelect
    (void)dt;
}

void Game::updateLevelSelect(float dt) {
    // TODO: 选关 + 确认 → startStage(level), m_state = Playing
    (void)dt;
}

void Game::updatePlaying(float dt) {
    if (!m_stageCtrl) return;
    StageEvent ev = m_stageCtrl->update(dt, m_input.getDirection());

    if (m_input.isPausePressed()) {
        m_state = GameState::Paused;
        return;
    }

    switch (ev) {
    case StageEvent::Continue:
        break;
    case StageEvent::LifeLost:
        m_state = GameState::LifeLost;
        break;
    case StageEvent::StageClear:
        m_state = GameState::StageClear;
        break;
    }
}

void Game::updatePaused(float dt) {
    (void)dt;
    if (m_input.isPausePressed() || m_input.isConfirmPressed()) {
        m_state = GameState::Playing;
    }
}

void Game::updateLifeLost(float dt) {
    (void)dt;
    // TODO: 短暂动画后:
    //   有命(m_score.lives > 0) → Playing
    //   无命 → GameOver
}

void Game::updateStageClear(float dt) {
    (void)dt;
    // TODO: 短暂动画后:
    //   非第9关: 解锁下一关 → LevelSelect
    //   第9关: → GameWin
}

void Game::updateGameOver(float dt) {
    (void)dt;
    // TODO: 确认键 → m_state = Menu
}

void Game::updateGameWin(float dt) {
    (void)dt;
    // TODO: 确认键 → m_state = Menu
}

void Game::startStage(int level) {
    destroyStage();
    // TODO: 根据 level 创建 StageData，然后 new StageController(...)
    (void)level;
}

void Game::destroyStage() {
    delete m_stageCtrl;
    m_stageCtrl = nullptr;
}
