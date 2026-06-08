module;

module game;

namespace game {

// ---- Board ----
Board::Board() : m_score(0) {}

void Board::reset() {}

void Board::spawnTile() {}

bool Board::move(Direction dir) { return false; }

bool Board::canMove() const { return false; }

bool Board::hasWon() const { return false; }

int Board::getTile(int row, int col) const { return m_grid[row][col].value; }

void Board::setTile(int row, int col, int value) { m_grid[row][col].value = value; }

std::vector<std::pair<int, int>> Board::getEmptyTiles() const { return {}; }

int Board::getScore() const { return m_score; }

bool Board::slideRow(int row, Direction dir) { return false; }

bool Board::mergeRow(int row, Direction dir) { return false; }

// ---- Game ----
Game::Game() : m_state(GameState::Menu) {}

void Game::reset() {}

void Game::move(Direction dir) {}

GameState Game::getState() const { return m_state; }

Board& Game::getBoard() { return m_board; }

const Board& Game::getBoard() const { return m_board; }

int Game::getScore() const { return m_board.getScore(); }

void Game::setState(GameState state) { m_state = state; }

} // namespace game
