#pragma once

#include <vector>
#include "../model/types.hpp"
#include "../model/map.hpp"
#include "../model/player.hpp"
#include "../model/ghost.hpp"

class Collision {
public:
    // 检查玩家是否吃到豆子，返回被吃豆子在 beans 列表中的索引（-1 表示没吃到）
    int  checkBean(const Player& player,
                   const std::vector<Pos>& beans,
                   const Map& map);

    // 玩家与幽灵是否碰撞
    bool checkPlayerGhost(const Player& player, const Ghost& ghost);

    // 从 pos 向 dir 走一步是否可行（目标格不是墙）
    bool canTurn(const Pos& pos, Direction dir, const Map& map);

    // 指定格子是否为墙
    bool isWall(const Pos& pos, const Map& map);
};
