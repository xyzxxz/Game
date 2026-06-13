#pragma once

#include <vector>

/**
 * @brief 单个格子的状态枚举
 */
enum class CellState {
    Hidden,    ///< 未翻开状态
    Revealed,  ///< 已翻开状态
    Flagged    ///< 已插旗状态
};

/**
 * @brief 单个格子的数据结构
 */
struct Cell {
    bool isMine = false;           ///< 是否为地雷
    CellState state = CellState::Hidden;  ///< 当前状态
    int adjacentMines = 0;         ///< 周围地雷数量
};

/**
 * @brief 扫雷棋盘类
 * 
 * 负责管理棋盘的状态、地雷放置、格子翻开等核心游戏逻辑
 */
class Board {
public:
    /**
     * @brief 默认构造函数
     */
    Board() = default;

    /**
     * @brief 初始化棋盘
     * @param width 棋盘宽度
     * @param height 棋盘高度
     * @param mines 地雷数量
     */
    void init(int width, int height, int mines);

    /**
     * @brief 获取棋盘宽度
     * @return 宽度值
     */
    int width() const;

    /**
     * @brief 获取棋盘高度
     * @return 高度值
     */
    int height() const;

    /**
     * @brief 获取地雷总数
     * @return 地雷数量
     */
    int totalMines() const;

    /**
     * @brief 获取剩余待翻开的安全格子数
     * @return 剩余安全格子数
     */
    int remaining() const;

    /**
     * @brief 获取指定位置的格子（只读）
     * @param x 横坐标
     * @param y 纵坐标
     * @return 格子常量引用
     */
    const Cell& at(int x, int y) const;

    /**
     * @brief 获取指定位置的格子（可写）
     * @param x 横坐标
     * @param y 纵坐标
     * @return 格子引用
     */
    Cell& at(int x, int y);

    /**
     * @brief 翻开指定格子
     * @param x 横坐标
     * @param y 纵坐标
     */
    void reveal(int x, int y);

    /**
     * @brief 切换指定格子的旗子状态
     * @param x 横坐标
     * @param y 纵坐标
     */
    void toggleFlag(int x, int y);

    /**
     * @brief 判断是否胜利
     * @return 是否所有安全格子都已翻开
     */
    bool isWin() const;

    /**
     * @brief 判断是否踩雷
     * @return 是否有地雷被翻开
     */
    bool isMineRevealed() const;

    /**
     * @brief 遍历所有格子（用于渲染）
     * @tparam Fn 回调函数类型
     * @param fn 回调函数，参数为(x, y, cell)
     */
    template <typename Fn>
    void forEachCell(Fn&& fn) const {
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                fn(x, y, m_grid[y * m_width + x]);
            }
        }
    }

private:
    /**
     * @brief 检查坐标是否在棋盘范围内
     * @param x 横坐标
     * @param y 纵坐标
     * @return 是否有效
     */
    bool isValid(int x, int y) const;

    /**
     * @brief 随机放置地雷
     */
    void placeMines();

    /**
     * @brief 计算每个格子周围的地雷数
     */
    void calculateAdjacentMines();

    /**
     * @brief 计算指定格子周围的地雷数
     * @param x 横坐标
     * @param y 纵坐标
     * @return 相邻地雷数量
     */
    int countAdjacentMines(int x, int y) const;

    /**
     * @brief 洪水填充算法，展开空白区域
     * @param x 起始横坐标
     * @param y 起始纵坐标
     */
    void floodFill(int x, int y);

    // 常量定义
    static constexpr char MINE_CHAR = '*';   ///< 地雷显示字符
    static constexpr char FLAG_CHAR = '!';   ///< 旗子显示字符

    // 成员变量
    int m_width = 0;          ///< 棋盘宽度
    int m_height = 0;         ///< 棋盘高度
    int m_totalMines = 0;     ///< 地雷总数
    int m_revealedCount = 0;  ///< 已翻开的格子数
    bool m_mineRevealed = false; ///< 是否踩雷

    std::vector<Cell> m_grid; ///< 格子数组
};
