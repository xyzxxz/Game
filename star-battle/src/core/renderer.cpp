#include "renderer.hpp"
#include "window.hpp"
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

namespace core {

Renderer::Renderer(SDL_Window* window)
{
    auto* handle = SDL_CreateRenderer(window, nullptr);
    if (!handle) {
        throw std::runtime_error(SDL_GetError());
    }
    m_handle.reset(handle);

    SDL_SetRenderLogicalPresentation(m_handle.get(),
        kDefaultWindowWidth, kDefaultWindowHeight,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

SDL_Renderer* Renderer::handle() const noexcept { return m_handle.get(); }

TexturePtr Renderer::load_texture(std::string_view path)
{
    auto* surface = IMG_Load(path.data());
    if (!surface) {
        SDL_Log("IMG_Load(%s) failed: %s", path.data(), SDL_GetError());
        return nullptr;
    }

    auto* texture = SDL_CreateTextureFromSurface(m_handle.get(), surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return nullptr;
    }

    return TexturePtr{texture};
}

void Renderer::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) noexcept
{
    SDL_SetRenderDrawColor(m_handle.get(), r, g, b, a);
}

void Renderer::clear() noexcept
{
    SDL_RenderClear(m_handle.get());
}

void Renderer::present() noexcept
{
    SDL_RenderPresent(m_handle.get());
}

} // namespace core