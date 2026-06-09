#pragma once

#include <vector>
#include "types.hpp"

class Map {
public:
    Map(int width, int height, int tileSize, const std::vector<std::vector<TileType>>& grid);

    int  width()    const { return m_width; }
    int  height()   const { return m_height; }
    int  tileSize() const { return m_tileSize; }

    TileType at(int x, int y) const;
    bool     isWalkable(int x, int y) const;

private:
    int                             m_width;
    int                             m_height;
    int                             m_tileSize;
    std::vector<std::vector<TileType>> m_grid;
};
