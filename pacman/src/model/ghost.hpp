#pragma once

#include "types.hpp"

struct Ghost {
    int        id       = 0;
    Pos        pos      = { 0, 0 };
    Direction  dir      = Direction::None;
    float      speed    = 3.0f;
    GhostState state    = GhostState::Chase;
    Pos        homePos  = { 0, 0 };
};
