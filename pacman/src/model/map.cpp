#include "../model/map.hpp"

Map::Map(int width, int height, int tileSize, const std::vector<std::vector<TileType>>& grid)
    : m_width(width), m_height(height), m_tileSize(tileSize), m_grid(grid) {}

TileType Map::at(int x, int y) const {
    // TODO
    return TileType::Wall;
}

bool Map::isWalkable(int x, int y) const {
    // TODO
    return false;
}
