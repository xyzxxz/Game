#include "Game.h"

Game::Game()
    : m_state(GameState::Menu)
    , m_quit(false)
    , m_paused(false)
    , m_window(nullptr)
    , m_renderer(nullptr) {
}

Game::~Game() {
}

bool Game::Init(SDL_Window* window, SDL_Renderer* renderer) {
    m_window   = window;
    m_renderer = renderer;

    if (!m_background.Init(renderer)) {
        SDL_Log("Background::Init failed");
        return false;
    }
    if (!m_bird.Init(renderer)) {
        SDL_Log("Bird::Init failed");
        return false;
    }
    if (!m_pipes.Init(renderer)) {
        SDL_Log("PipeManager::Init failed");
        return false;
    }
    if (!m_ground.Init(renderer)) {
        SDL_Log("Ground::Init failed");
        return false;
    }
    if (!m_score.Init(renderer)) {
        SDL_Log("Score::Init failed");
        return false;
    }
    if (!m_ui.Init(renderer)) {
        SDL_Log("UI::Init failed");
        return false;
    }

    m_audio.Init();  // 音频非关键，失败也不阻止游戏启动

    return true;
}

// ========== 事件分发 ==========

void Game::HandleEvent(const SDL_Event* event) {
    switch (m_state) {
        case GameState::Menu:     HandleMenuEvent(event);     break;
        case GameState::Playing:  HandlePlayingEvent(event);  break;
        case GameState::GameOver: HandleGameOverEvent(event); break;
    }
}

// ========== 更新 ==========

void Game::Update(float dt) {
    // 背景一直在滚动
    m_background.Update(dt);
    m_ground.Update(dt);

    if (m_state == GameState::Playing && !m_paused) {
        UpdatePlaying(dt);
    }
}

// ========== 渲染 ==========

void Game::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 78, 192, 202, 255);
    SDL_RenderClear(renderer);

    m_background.Render(renderer);

    switch (m_state) {
        case GameState::Menu:
            m_ground.Render(renderer);
            m_bird.Render(renderer);  // 菜单时小鸟也显示
            m_ui.RenderMenu(renderer);
            break;

        case GameState::Playing:
            m_pipes.Render(renderer);
            m_ground.Render(renderer);
            m_bird.Render(renderer);
            m_ui.RenderHUD(renderer, &m_score);
            break;

        case GameState::GameOver:
            m_pipes.Render(renderer);
            m_ground.Render(renderer);
            m_bird.Render(renderer);
            m_ui.RenderGameOver(renderer, &m_score);
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::Quit() {
    // 各子系统的析构函数会自动清理
}

bool Game::ShouldQuit() const {
    return m_quit;
}

// ========== 菜单事件处理 ==========

void Game::HandleMenuEvent(const SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_SPACE) {
            StartGame();
            return;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            float mx = event->button.x;
            float my = event->button.y;
            if (m_ui.IsStartButtonClicked(mx, my)) {
                StartGame();
            }
        }
    }
}

// ========== 游戏中事件处理 ==========

void Game::HandlePlayingEvent(const SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_SPACE:
                if (m_paused) {
                    m_paused = false;
                } else {
                    m_bird.Flap();
                    m_audio.PlayWing();
                }
                break;
            case SDLK_P:
                m_paused = !m_paused;
                break;
            case SDLK_ESCAPE:
                m_quit = true;
                break;
        }
        return;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            if (m_paused) {
                m_paused = false;
            } else {
                m_bird.Flap();
                m_audio.PlayWing();
            }
        }
    }
}

// ========== 结束界面事件处理 ==========

void Game::HandleGameOverEvent(const SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_SPACE) {
            RestartGame();
            return;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            float mx = event->button.x;
            float my = event->button.y;
            if (m_ui.IsOkButtonClicked(mx, my)) {
                RestartGame();
            }
        }
    }
}

// ========== 游戏逻辑更新 ==========

void Game::UpdatePlaying(float dt) {
    m_bird.Update(dt);
    m_pipes.Update(dt);

    // 计分：小鸟通过管道
    auto& pipes = m_pipes.GetPipes();
    for (auto& pipe : pipes) {
        if (!pipe.scored) {
            float pipeRight = pipe.x + kPipeWidth;
            float birdCenterX = m_bird.GetX() + m_bird.GetWidth() / 2.0f;
            if (birdCenterX > pipeRight) {
                pipe.scored = true;
                m_score.SetScore(m_score.GetScore() + 1);
                m_audio.PlayPoint();
            }
        }
    }

    // 碰撞检测：撞管道
    if (m_pipes.CheckCollision(m_bird.GetX(), m_bird.GetY(),
                                m_bird.GetWidth(), m_bird.GetHeight())) {
        OnGameOver();
        return;
    }

    // 碰撞检测：撞地面或飞出屏幕顶部
    float groundY = m_ground.GetY();
    if (m_bird.GetY() + m_bird.GetHeight() >= groundY) {
        OnGameOver();
        return;
    }

    if (m_bird.GetY() <= 0) {
        OnGameOver();
        return;
    }
}

// ========== 状态切换 ==========

void Game::StartGame() {
    m_bird.Reset();
    m_pipes.Reset();
    m_score.Reset();
    m_ground.Reset();
    m_state = GameState::Playing;
    m_paused = false;
}

void Game::OnGameOver() {
    m_state = GameState::GameOver;
    m_audio.PlayHit();
    m_audio.PlayDie();
}

void Game::RestartGame() {
    m_state = GameState::Menu;
}