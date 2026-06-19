#include "pch.hpp"
#include <SDL3/SDL_main.h>
#include "star_battle_game.hpp"
#include <cstdlib>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    StarBattleGame game{"Star Battle"};
    return game.run();
}