#include "game.h"

#include <algorithm>
#include <random>
#include <vector>

namespace game {

// ============================================================
//  Board 实现
// ============================================================

Board::Board()
    : m_score(0)
{
    // 初始化随机数生成器
    std::random_device rd;
    m_rng.seed(rd());
}

// ---- 重置棋盘 ----
void Board::reset()
{
    // 清空所有格子
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            m_grid[r][c].value = 0;

    m_score = 0;

    // 开局生成两个方块
    spawnTile();
    spawnTile();
}

// ---- 在空位随机生成新方块 ----
void Board::spawnTile()
{
    auto empties = getEmptyTiles();
    if (empties.empty()) return;

    // 随机选一个空位
    std::uniform_int_distribution<size_t> distIndex(0, empties.size() - 1);
    auto [row, col] = empties[distIndex(m_rng)];

    // 90% 概率出 2，10% 出 4
    std::uniform_int_distribution<int> distValue(1, 10);
    m_grid[row][col].value = (distValue(m_rng) <= 9) ? 2 : 4;
}

// ---- 核心：处理一行，滑动 + 合并 ----
bool Board::processLine(int line[SIZE], int& scoreDelta)
{
    bool changed = false;

    // 第一步：将所有非零值压缩到索引 0 方向
    int write = 0;
    for (int read = 0; read < SIZE; ++read) {
        if (line[read] != 0) {
            if (read != write) {
                line[write] = line[read];
                line[read]  = 0;
                changed = true;
            }
            ++write;
        }
    }

    // 第二步：合并相邻的相等值
    for (int i = 0; i < SIZE - 1; ++i) {
        if (line[i] != 0 && line[i] == line[i + 1]) {
            line[i] *= 2;
            scoreDelta += line[i];
            line[i + 1] = 0;
            changed = true;
        }
    }

    // 第三步：再次压缩，填补合并后产生的空隙
    write = 0;
    for (int read = 0; read < SIZE; ++read) {
        if (line[read] != 0) {
            if (read != write) {
                line[write] = line[read];
                line[read]  = 0;
                changed = true;
            }
            ++write;
        }
    }

    return changed;
}

// ---- 向指定方向移动 ----
bool Board::move(Direction dir)
{
    int  line[SIZE];
    bool changed = false;
    int  scoreDelta = 0;

    switch (dir) {
    case Direction::Left: {
        // 每行从左向右压缩
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c)
                line[c] = m_grid[r][c].value;

            if (processLine(line, scoreDelta)) {
                changed = true;
                for (int c = 0; c < SIZE; ++c)
                    m_grid[r][c].value = line[c];
            }
        }
        break;
    }
    case Direction::Right: {
        // 每行从右向左压缩：先反向取，处理后反向写回
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c)
                line[c] = m_grid[r][SIZE - 1 - c].value;

            if (processLine(line, scoreDelta)) {
                changed = true;
                for (int c = 0; c < SIZE; ++c)
                    m_grid[r][SIZE - 1 - c].value = line[c];
            }
        }
        break;
    }
    case Direction::Up: {
        // 每列从上向下压缩
        for (int c = 0; c < SIZE; ++c) {
            for (int r = 0; r < SIZE; ++r)
                line[r] = m_grid[r][c].value;

            if (processLine(line, scoreDelta)) {
                changed = true;
                for (int r = 0; r < SIZE; ++r)
                    m_grid[r][c].value = line[r];
            }
        }
        break;
    }
    case Direction::Down: {
        // 每列从下向上压缩
        for (int c = 0; c < SIZE; ++c) {
            for (int r = 0; r < SIZE; ++r)
                line[r] = m_grid[SIZE - 1 - r][c].value;

            if (processLine(line, scoreDelta)) {
                changed = true;
                for (int r = 0; r < SIZE; ++r)
                    m_grid[SIZE - 1 - r][c].value = line[r];
            }
        }
        break;
    }
    }

    m_score += scoreDelta;
    return changed;
}

// ---- 是否还能移动 ----
bool Board::canMove() const
{
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (m_grid[r][c].value == 0) return true;        // 存在空位

            // 检查右侧相邻
            if (c < SIZE - 1 && m_grid[r][c].value == m_grid[r][c + 1].value)
                return true;
            // 检查下方相邻
            if (r < SIZE - 1 && m_grid[r][c].value == m_grid[r + 1][c].value)
                return true;
        }
    }
    return false;
}

// ---- 是否达到 2048 ----
bool Board::hasWon() const
{
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (m_grid[r][c].value >= 2048)
                return true;
    return false;
}

// ---- 获得所有空位的坐标 ----
std::vector<std::pair<int, int>> Board::getEmptyTiles() const
{
    std::vector<std::pair<int, int>> result;
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (m_grid[r][c].value == 0)
                result.emplace_back(r, c);
    return result;
}

// ---- 访问接口 ----
int Board::getTile(int row, int col) const
{
    return m_grid[row][col].value;
}

void Board::setTile(int row, int col, int value)
{
    m_grid[row][col].value = value;
}

int Board::getScore() const
{
    return m_score;
}

// ============================================================
//  Game 实现
// ============================================================

Game::Game()
    : m_state(GameState::Menu)
{}

void Game::reset()
{
    m_board.reset();
    m_state = GameState::Playing;
}

void Game::move(Direction dir)
{
    // 非 Playing 状态忽略移动
    if (m_state != GameState::Playing) return;

    bool moved = m_board.move(dir);
    if (!moved) return;

    // 移动有效，生成新方块
    m_board.spawnTile();

    // 检查胜负条件
    if (m_board.hasWon()) {
        m_state = GameState::Won;
    } else if (!m_board.canMove()) {
        m_state = GameState::Lost;
    }
}

GameState Game::getState() const { return m_state; }

Board& Game::getBoard() { return m_board; }

const Board& Game::getBoard() const { return m_board; }

int Game::getScore() const { return m_board.getScore(); }

void Game::setState(GameState state) { m_state = state; }

} // namespace game
