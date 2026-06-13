#pragma once

enum class Difficulty {
    Easy,
    Normal,
    Hard
};

struct DifficultyConfig {
    int width;
    int height;
    int mines;
    const char* name;
};

inline DifficultyConfig getDifficultyConfig(Difficulty diff) {
    switch (diff) {
    case Difficulty::Easy:   return {10, 10, 10,  "简单"};
    case Difficulty::Normal: return {15, 15, 50,  "普通"};
    case Difficulty::Hard:   return {20, 20, 100, "困难"};
    }
    return {10, 10, 10, "简单"};
}
