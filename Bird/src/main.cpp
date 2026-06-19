#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Game.h"

// ========== 全局应用状态 ==========
struct AppState {
    SDL_Window*   window;
    SDL_Renderer* renderer;
    Game*         game;
};

// ========== 初始化 ==========
SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    auto* state = new AppState{};
    *appstate = state;

    SDL_SetAppMetadata("Flappy Bird", "1.0", "com.example.flappybird");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = SDL_CreateWindow("Flappy Bird", kScreenWidth, kScreenHeight, 0);
    if (!state->window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->renderer = SDL_CreateRenderer(state->window, nullptr);
    if (!state->renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(state->renderer,
        static_cast<int>(kLogicalWidth),
        static_cast<int>(kLogicalHeight),
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    state->game = new Game();
    if (!state->game->Init(state->window, state->renderer)) {
        SDL_Log("Game::Init failed");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

// ========== 事件处理 ==========
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    auto* state = static_cast<AppState*>(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    state->game->HandleEvent(event);

    return SDL_APP_CONTINUE;
}

// ========== 帧更新与渲染 ==========
SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* state = static_cast<AppState*>(appstate);

    // 固定时间步长
    static Uint64 lastTime = SDL_GetTicksNS();
    Uint64 now = SDL_GetTicksNS();
    float dt = static_cast<float>(now - lastTime) / 1'000'000'000.0f;
    lastTime = now;

    // 限制最大帧时间避免跳跃
    if (dt > 0.1f) dt = 0.1f;

    state->game->Update(dt);
    state->game->Render(state->renderer);

    if (state->game->ShouldQuit()) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

// ========== 清理 ==========
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    auto* state = static_cast<AppState*>(appstate);

    state->game->Quit();
    delete state->game;

    if (state->renderer) SDL_DestroyRenderer(state->renderer);
    if (state->window) SDL_DestroyWindow(state->window);

    delete state;
}