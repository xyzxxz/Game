#pragma once

#include <SDL3/SDL.h>
#include "../resource.hpp"
#include "../model/score.hpp"

class HudRenderer {
public:
    void render(SDL_Renderer* renderer, ResourceManager& res,
                const Score& score);
};
