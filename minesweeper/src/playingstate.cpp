#include "playingstate.hpp"
#include "game.hpp"
#include "gameoverstate.hpp"
#include "menustate.hpp"
#include "render_text.hpp"
#include "difficulty.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include <string>

PlayingState::PlayingState(Game& game, Difficulty difficulty)
    : GameState(game) {
    auto config = getDifficultyConfig(difficulty);
    m_board.init(config.width, config.height, config.mines);
}

void PlayingState::enter() {}
void PlayingState::exit() {}

void PlayingState::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            m_game.changeState(std::make_unique<MenuState>(m_game));
        }
    }
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        
        SDL_Window* window = m_game.window();
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);
        
        int boardW = m_board.width();
        int boardH = m_board.height();
        
        int cellSize = std::min(winW / boardW, winH / boardH);
        int offsetX = (winW - boardW * cellSize) / 2;
        int offsetY = (winH - boardH * cellSize) / 2;
        
        int cellX = static_cast<int>((x - static_cast<float>(offsetX)) / static_cast<float>(cellSize));
        int cellY = static_cast<int>((y - static_cast<float>(offsetY)) / static_cast<float>(cellSize));
        
        if (cellX >= 0 && cellX < boardW && cellY >= 0 && cellY < boardH) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_board.reveal(cellX, cellY);
                if (m_board.isMineRevealed()) {
                    m_game.changeState(std::make_unique<GameOverState>(m_game, GameResult::Lose));
                } else if (m_board.isWin()) {
                    m_game.changeState(std::make_unique<GameOverState>(m_game, GameResult::Win));
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                m_board.toggleFlag(cellX, cellY);
            }
        }
    }
}

void PlayingState::update() {}

void PlayingState::render() {
    SDL_Renderer* renderer = m_game.renderer();
    TTF_Font* font = m_game.getFont();
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    
    SDL_Window* window = m_game.window();
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    
    int boardW = m_board.width();
    int boardH = m_board.height();
    
    int cellSize = std::min(winW / boardW, winH / boardH);
    int offsetX = (winW - boardW * cellSize) / 2;
    int offsetY = (winH - boardH * cellSize) / 2;
    
    SDL_Color colors[] = {
        {0x00, 0x00, 0x00, 0xff}, // 0
        {0x34, 0x98, 0xdb, 0xff}, // 1
        {0x27, 0xae, 0x60, 0xff}, // 2
        {0xe7, 0x4c, 0x3c, 0xff}, // 3
        {0x9b, 0x59, 0xb6, 0xff}, // 4
        {0xf3, 0x9c, 0x12, 0xff}, // 5
        {0x16, 0xa0, 0x85, 0xff}, // 6
        {0xc0, 0x39, 0x2b, 0xff}, // 7
        {0x7f, 0x8c, 0x8d, 0xff}  // 8
    };
    
    m_board.forEachCell([&](int x, int y, const Cell& cell) {
        int px = offsetX + x * cellSize;
        int py = offsetY + y * cellSize;
        
        SDL_FRect rect = {static_cast<float>(px), static_cast<float>(py), 
                         static_cast<float>(cellSize - 1), static_cast<float>(cellSize - 1)};
        
        if (cell.state == CellState::Hidden) {
            SDL_SetRenderDrawColor(renderer, 0x95, 0xa5, 0xa6, 0xff);
            SDL_RenderFillRect(renderer, &rect);
        } else if (cell.state == CellState::Flagged) {
            SDL_SetRenderDrawColor(renderer, 0xe7, 0x4c, 0x3c, 0xff);
            SDL_RenderFillRect(renderer, &rect);
            renderTextCentered(renderer, font, "F", {0xff, 0xff, 0xff, 0xff}, px + cellSize/2, py + cellSize/2);
        } else {
            SDL_SetRenderDrawColor(renderer, 0xbd, 0xc3, 0xc7, 0xff);
            SDL_RenderFillRect(renderer, &rect);
            
            if (cell.isMine) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
                renderTextCentered(renderer, font, "*", {0x00, 0x00, 0x00, 0xff}, px + cellSize/2, py + cellSize/2);
            } else if (cell.adjacentMines > 0) {
                char num[2] = { static_cast<char>('0' + cell.adjacentMines), '\0' };
                renderTextCentered(renderer, font, num, colors[cell.adjacentMines], px + cellSize/2, py + cellSize/2);
            }
        }
    });
    
    SDL_RenderPresent(renderer);
}
