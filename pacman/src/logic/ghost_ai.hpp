#pragma once

#include "../model/types.hpp"
#include "../model/map.hpp"
#include "../model/player.hpp"
#include "../model/ghost.hpp"

class GhostAI {
public:
    // 决定幽灵的下一步方向
    Direction decide(const Ghost& ghost, const Player& player,
                     const Map& map);
};
