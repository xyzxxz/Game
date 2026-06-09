#pragma once

#include "types.hpp"

struct Player {
    Pos       pos       = { 0, 0 };
    Direction dir       = Direction::None;
    Direction nextDir   = Direction::None;
    float     speed     = 5.0f;     // 格/秒
    int       lives     = 3;
    bool      invincible = false;
};
