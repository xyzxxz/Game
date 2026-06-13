#pragma once
#include <array>
#include <random>

class Board {
public:
    Board();

    int  get(int row, int col) const;
    void set(int row, int col, int val);
    bool isFixed(int row, int col) const;
    bool isComplete() const;
    bool hasConflict(int row, int col) const;

    void generate();

private:
    std::array<std::array<int, 9>, 9> grid_{};
    std::array<std::array<bool, 9>, 9> fixed_{};
    std::mt19937 rng_{std::random_device{}()};

    bool solve();
    bool solveFrom(int row, int col);
    bool isSafe(int row, int col, int val) const;
};