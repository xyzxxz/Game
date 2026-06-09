#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    explicit ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    SDL_Texture* getTexture(const std::string& key);
    void         loadAll();
    void         releaseAll();

private:
    SDL_Renderer* m_renderer;
    std::unordered_map<std::string, SDL_Texture*> m_cache;

    SDL_Texture* loadTexture(const char* path);
};
