#pragma once

#include "gamestate.hpp"
#include "board.hpp"
#include "difficulty.hpp"

class PlayingState : public GameState {
public:
    PlayingState(Game& game, Difficulty difficulty);

    void enter()  override;
    void exit()   override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render() override;

private:
    Board m_board;
    bool  m_firstClick = true;   // 首次点击后才布雷（保证不踩雷）
};
