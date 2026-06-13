#include "game.hpp"
#include "menustate.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

Game::Game() {}
Game::~Game() {}

bool Game::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL Init failed.");
    }

    SDL_CreateWindowAndRenderer("扫雷", 800, 800, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);

    if (m_window == nullptr || m_renderer == nullptr) {
        SDL_Log("Create window or renderer failed.");
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed.");
    }

    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        SDL_Log("Open font failed.");
    }

    m_running = true;

    // 初始进入菜单状态
    changeState(std::make_unique<MenuState>(*this));
    return true;
}

void Game::run() {
    while (m_running) {
        handleEvents();
        update();
        render();
    }
}

void Game::changeState(std::unique_ptr<GameState> newState) {
    if (m_currentState) {
        m_currentState->exit();
    }
    m_currentState = std::move(newState);
    if (m_currentState) {
        m_currentState->enter();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
            return;
        }
        if (m_currentState) {
            m_currentState->handleEvent(event);
        }
    }
}

void Game::update() {
    if (m_currentState) {
        m_currentState->update();
    }
}

void Game::render() {
    if (m_currentState) {
        m_currentState->render();
    }
}
