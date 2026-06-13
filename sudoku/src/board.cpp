#include "board.h"
#include <algorithm>
#include <numeric>
#include <vector>

Board::Board() { generate(); }

void Board::generate() {
    for (auto& row : grid_) row.fill(0);
    for (auto& row : fixed_) row.fill(false);

    solve();

    // Remove cells randomly, keeping ~40 clues
    std::vector<int> pos(81);
    std::iota(pos.begin(), pos.end(), 0);
    std::shuffle(pos.begin(), pos.end(), rng_);

    for (int i = 0; i < 81 - 40; ++i)
        grid_[pos[i] / 9][pos[i] % 9] = 0;

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (grid_[r][c] != 0)
                fixed_[r][c] = true;
}

int Board::get(int row, int col) const { return grid_[row][col]; }

void Board::set(int row, int col, int val) {
    if (!fixed_[row][col])
        grid_[row][col] = val;
}

bool Board::isFixed(int row, int col) const { return fixed_[row][col]; }

bool Board::isComplete() const {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (grid_[r][c] == 0)
                return false;

    // Validate all rows, columns, boxes
    auto check = [](auto&& nums) {
        int mask = 0;
        for (int n : nums)
            mask |= 1 << n;
        return mask == 0x3FE; // bits 1..9
    };

    for (int r = 0; r < 9; ++r)
        if (!check(grid_[r])) return false;

    for (int c = 0; c < 9; ++c) {
        int mask = 0;
        for (int r = 0; r < 9; ++r)
            mask |= 1 << grid_[r][c];
        if (mask != 0x3FE) return false;
    }

    for (int br = 0; br < 9; br += 3)
        for (int bc = 0; bc < 9; bc += 3) {
            int mask = 0;
            for (int r = br; r < br + 3; ++r)
                for (int c = bc; c < bc + 3; ++c)
                    mask |= 1 << grid_[r][c];
            if (mask != 0x3FE) return false;
        }

    return true;
}

bool Board::hasConflict(int row, int col) const {
    int val = grid_[row][col];
    if (val == 0) return false;

    for (int i = 0; i < 9; ++i) {
        if (i != col && grid_[row][i] == val) return true;
        if (i != row && grid_[i][col] == val) return true;
    }

    int br = row / 3 * 3, bc = col / 3 * 3;
    for (int r = br; r < br + 3; ++r)
        for (int c = bc; c < bc + 3; ++c)
            if ((r != row || c != col) && grid_[r][c] == val)
                return true;

    return false;
}

bool Board::solve() { return solveFrom(0, 0); }

bool Board::solveFrom(int row, int col) {
    if (row == 9) return true;
    if (col == 9) return solveFrom(row + 1, 0);
    if (grid_[row][col] != 0) return solveFrom(row, col + 1);

    std::array<int, 9> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(nums.begin(), nums.end(), rng_);

    for (int num : nums) {
        if (isSafe(row, col, num)) {
            grid_[row][col] = num;
            if (solveFrom(row, col + 1)) return true;
            grid_[row][col] = 0;
        }
    }
    return false;
}

bool Board::isSafe(int row, int col, int val) const {
    for (int i = 0; i < 9; ++i)
        if (grid_[row][i] == val || grid_[i][col] == val)
            return false;

    int br = row / 3 * 3, bc = col / 3 * 3;
    for (int r = br; r < br + 3; ++r)
        for (int c = bc; c < bc + 3; ++c)
            if (grid_[r][c] == val)
                return false;

    return true;
}