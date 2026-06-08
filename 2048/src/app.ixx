module;

#include <SDL3/SDL.h>

export module app;

import game;
import renderer;

namespace app {

// ---- 菜单选项 ----
export enum class MenuOption {
    Start,
    Quit,
    Count
};

// ---- 应用程序主控 ----
export class App {
public:
    App();
    ~App();

    // 启动应用，返回退出码
    int run();

private:
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    int           m_windowW;
    int           m_windowH;

    game::Game                m_game;
    renderer::BoardRenderer   m_boardRenderer;
    renderer::MenuRenderer    m_menuRenderer;
    renderer::HUDRenderer     m_hudRenderer;

    MenuOption    m_menuSelection;
    bool          m_running;

    // 事件分发
    void handleMenuEvent(const SDL_Event& event);
    void handleGameEvent(const SDL_Event& event);
    void handleEvent(const SDL_Event& event);

    // 帧更新
    void update(float dt);

    // 帧渲染
    void render();
};

} // namespace app
