#pragma once

#include "gamestate.hpp"
#include <string_view>

enum class GameResult {
    Win,
    Lose
};

class GameOverState : public GameState {
public:
    GameOverState(Game& game, GameResult result);

    void enter()  override;
    void exit()   override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render() override;

private:
    GameResult m_result;
    std::string_view m_win = "恭喜你，你赢了！";
    std::string_view m_lose = "很遗憾，你输了。";
};
