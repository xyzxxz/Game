#pragma once

#include "map.hpp"
#include "player.hpp"
#include "ghost.hpp"

struct StageData {
    int       levelNum       = 1;
    Map       map;
    Pos       playerStart    = { 0, 0 };
    Ghost     ghosts[4];
    float     ghostBaseSpeed = 3.0f;
    int       powerBeanCount = 0;
};
