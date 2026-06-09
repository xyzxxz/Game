#pragma once

#include <SDL3/SDL.h>
#include "../resource.hpp"

class MenuRenderer {
public:
    void render(SDL_Renderer* renderer, ResourceManager& res);
    int  selectedItem() const { return m_selected; }

private:
    int m_selected = 0;
};
