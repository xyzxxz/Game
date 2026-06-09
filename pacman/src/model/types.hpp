#pragma once

enum class Direction { Up, Down, Left, Right, None };

enum class TileType { Wall, Bean, PowerBean, Empty, GhostHouse };

enum class GameState { Menu, LevelSelect, Playing, Paused, LifeLost, StageClear, GameOver, GameWin };

enum class GhostState { Chase, Scatter, Frightened, Eaten };

struct Pos {
    int x, y;
};
