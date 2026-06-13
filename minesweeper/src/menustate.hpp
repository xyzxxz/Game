#pragma once

#include "gamestate.hpp"
#include "difficulty.hpp"
#include <string_view>

class MenuState : public GameState {
public:
    MenuState(Game& game);

    void enter()  override;
    void exit()   override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render() override;

    Difficulty selectedDifficulty() const;

private:
    Difficulty m_selected = Difficulty::Easy;
    std::string_view m_title = "扫雷";
    std::string_view m_difficulty = "选择难度";
    std::string_view menu1 = "1. 简单";
    std::string_view menu2 = "2. 中等";
    std::string_view menu3 = "3. 困难";
    std::string_view m_start = "开始游戏";
};
