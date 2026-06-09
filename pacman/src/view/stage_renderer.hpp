#pragma once

#include <SDL3/SDL.h>
#include "../resource.hpp"
#include "../model/stage.hpp"

class StageRenderer {
public:
    void render(SDL_Renderer* renderer, ResourceManager& res,
                const StageData& stage, const Player& player,
                const Ghost ghosts[4]);
};
