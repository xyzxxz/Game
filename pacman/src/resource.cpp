#include "resource.hpp"

ResourceManager::ResourceManager(SDL_Renderer* renderer)
    : m_renderer(renderer) {}

ResourceManager::~ResourceManager() {
    releaseAll();
}

SDL_Texture* ResourceManager::getTexture(const std::string& key) {
    auto it = m_cache.find(key);
    return (it != m_cache.end()) ? it->second : nullptr;
}

SDL_Texture* ResourceManager::loadTexture(const char* path) {
    // TODO: 使用 SDL3_image 加载图片
    return nullptr;
}

void ResourceManager::loadAll() {
    // TODO: 加载全部图片资源
}

void ResourceManager::releaseAll() {
    for (auto& [key, tex] : m_cache) {
        SDL_DestroyTexture(tex);
    }
    m_cache.clear();
}
