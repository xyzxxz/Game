#include "board.hpp"
#include <random>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>

/**
 * @brief 初始化棋盘
 * @param width 棋盘宽度
 * @param height 棋盘高度
 * @param mines 地雷数量
 */
void Board::init(int width, int height, int mines) {
    // 重置所有成员变量
    m_width = width;
    m_height = height;
    m_totalMines = mines;
    m_revealedCount = 0;
    m_mineRevealed = false;
    
    // 调整地雷数量上限
    if (m_totalMines >= width * height) {
        m_totalMines = width * height - 1;
    }
    
    // 初始化格子数组
    m_grid.resize(width * height);
    for (auto& cell : m_grid) {
        cell.isMine = false;
        cell.state = CellState::Hidden;
        cell.adjacentMines = 0;
    }
    
    // 随机放置地雷
    placeMines();
    
    // 计算每个格子周围的地雷数
    calculateAdjacentMines();
}

/**
 * @brief 获取棋盘宽度
 * @return 宽度值
 */
int Board::width() const { return m_width; }

/**
 * @brief 获取棋盘高度
 * @return 高度值
 */
int Board::height() const { return m_height; }

/**
 * @brief 获取地雷总数
 * @return 地雷数量
 */
int Board::totalMines() const { return m_totalMines; }

/**
 * @brief 获取剩余待翻开的安全格子数
 * @return 剩余安全格子数
 */
int Board::remaining() const { 
    return m_width * m_height - m_totalMines - m_revealedCount; 
}

/**
 * @brief 获取指定位置的格子（只读）
 * @param x 横坐标
 * @param y 纵坐标
 * @return 格子引用
 */
const Cell& Board::at(int x, int y) const { 
    return m_grid[y * m_width + x]; 
}

/**
 * @brief 获取指定位置的格子（可写）
 * @param x 横坐标
 * @param y 纵坐标
 * @return 格子引用
 */
Cell& Board::at(int x, int y) { 
    return m_grid[y * m_width + x]; 
}

/**
 * @brief 检查坐标是否在棋盘范围内
 * @param x 横坐标
 * @param y 纵坐标
 * @return 是否有效
 */
bool Board::isValid(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

/**
 * @brief 翻开指定格子
 * @param x 横坐标
 * @param y 纵坐标
 */
void Board::reveal(int x, int y) {
    // 检查坐标有效性
    if (!isValid(x, y)) {
        return;
    }
    
    Cell& cell = at(x, y);
    
    // 如果已经翻开或插旗，直接返回
    if (cell.state == CellState::Revealed || cell.state == CellState::Flagged) {
        return;
    }
    
    // 标记为已翻开
    cell.state = CellState::Revealed;
    m_revealedCount++;
    
    // 如果是地雷，标记游戏失败
    if (cell.isMine) {
        m_mineRevealed = true;
        return;
    }
    
    // 如果周围没有地雷，递归展开相邻格子
    if (cell.adjacentMines == 0) {
        floodFill(x, y);
    }
}

/**
 * @brief 切换指定格子的旗子状态
 * @param x 横坐标
 * @param y 纵坐标
 */
void Board::toggleFlag(int x, int y) {
    // 检查坐标有效性
    if (!isValid(x, y)) {
        return;
    }
    
    Cell& cell = at(x, y);
    
    // 已翻开的格子不能插旗
    if (cell.state == CellState::Revealed) {
        return;
    }
    
    // 切换状态
    if (cell.state == CellState::Hidden) {
        cell.state = CellState::Flagged;
    } else if (cell.state == CellState::Flagged) {
        cell.state = CellState::Hidden;
    }
}

/**
 * @brief 判断是否胜利
 * @return 是否所有安全格子都已翻开
 */
bool Board::isWin() const { 
    // 胜利条件：所有非地雷格子都已翻开
    return m_revealedCount == m_width * m_height - m_totalMines;
}

/**
 * @brief 判断是否踩雷
 * @return 是否有地雷被翻开
 */
bool Board::isMineRevealed() const { return m_mineRevealed; }

/**
 * @brief 随机放置地雷
 */
void Board::placeMines() {
    // 创建位置池 [0, width*height)
    std::vector<int> pool(m_width * m_height);
    std::iota(pool.begin(), pool.end(), 0);
    
    // 使用随机数生成器
    std::random_device rd;
    std::mt19937 rng(rd());
    
    // 随机选择地雷位置
    std::vector<int> minePositions;
    minePositions.reserve(m_totalMines);
    std::sample(pool.begin(), pool.end(), std::back_inserter(minePositions), 
                m_totalMines, rng);
    
    // 标记地雷
    for (int idx : minePositions) {
        m_grid[idx].isMine = true;
    }
}

/**
 * @brief 计算每个格子周围的地雷数
 */
void Board::calculateAdjacentMines() {
    // 遍历所有格子
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            m_grid[y * m_width + x].adjacentMines = countAdjacentMines(x, y);
        }
    }
}

/**
 * @brief 计算指定格子周围的地雷数
 * @param x 横坐标
 * @param y 纵坐标
 * @return 相邻地雷数量
 */
int Board::countAdjacentMines(int x, int y) const {
    if (m_grid[y * m_width + x].isMine) {
        return -1; // 地雷本身返回-1
    }
    
    int count = 0;
    
    // 检查8个方向
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            // 跳过自身
            if (dx == 0 && dy == 0) {
                continue;
            }
            
            int nx = x + dx;
            int ny = y + dy;
            
            // 检查边界并计数地雷
            if (isValid(nx, ny) && m_grid[ny * m_width + nx].isMine) {
                count++;
            }
        }
    }
    
    return count;
}

/**
 * @brief 洪水填充算法，展开空白区域
 * @param x 起始横坐标
 * @param y 起始纵坐标
 */
void Board::floodFill(int x, int y) {
    // 检查边界
    if (!isValid(x, y)) {
        return;
    }
    
    Cell& cell = at(x, y);
    
    // 跳过已翻开、已插旗或有地雷的格子
    if (cell.state == CellState::Revealed || 
        cell.state == CellState::Flagged || 
        cell.isMine) {
        return;
    }
    
    // 标记为已翻开
    cell.state = CellState::Revealed;
    m_revealedCount++;
    
    // 如果周围没有地雷，继续递归展开
    if (cell.adjacentMines == 0) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx != 0 || dy != 0) {
                    floodFill(x + dx, y + dy);
                }
            }
        }
    }
}
