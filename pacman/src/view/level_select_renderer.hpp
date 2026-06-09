#pragma once

#include <SDL3/SDL.h>
#include "../resource.hpp"

class LevelSelectRenderer {
public:
    void render(SDL_Renderer* renderer, ResourceManager& res,
                int unlockedLevel);
    int  selectedLevel() const { return m_selected; }

private:
    int m_selected = 1;
};
