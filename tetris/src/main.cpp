#include "game.hpp"
#include "renderer.hpp"
#include <SDL3/SDL.h>

// ============================================================
// 程序入口：创建游戏与渲染器，启动主循环
// ============================================================

int main()
{
    Renderer renderer;
    if (!renderer.init()) {
        SDL_Log("渲染器初始化失败，程序退出");
        return 1;
    }

    Game game;
    game.run(renderer);

    return 0;
}