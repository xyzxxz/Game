#include "collision.hpp"

int Collision::checkBean(const Player& player,
                         const std::vector<Pos>& beans,
                         const Map& map) {
    // TODO
    (void)player;
    (void)beans;
    (void)map;
    return -1;
}

bool Collision::checkPlayerGhost(const Player& player, const Ghost& ghost) {
    // TODO
    (void)player;
    (void)ghost;
    return false;
}

bool Collision::canTurn(const Pos& pos, Direction dir, const Map& map) {
    // TODO
    (void)pos;
    (void)dir;
    (void)map;
    return false;
}

bool Collision::isWall(const Pos& pos, const Map& map) {
    // TODO
    (void)pos;
    (void)map;
    return false;
}
