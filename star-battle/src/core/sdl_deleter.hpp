#pragma once

#include <SDL3/SDL.h>
#include <memory>

namespace core {

/// @brief SDL3 资源 RAII 删除器，配合 std::unique_ptr 使用
struct SdlDeleter {
    void operator()(SDL_Window* ptr) const noexcept { SDL_DestroyWindow(ptr); }
    void operator()(SDL_Renderer* ptr) const noexcept { SDL_DestroyRenderer(ptr); }
    void operator()(SDL_Texture* ptr) const noexcept { SDL_DestroyTexture(ptr); }
    void operator()(SDL_Surface* ptr) const noexcept { SDL_DestroySurface(ptr); }
    void operator()(SDL_AudioStream* ptr) const noexcept { SDL_DestroyAudioStream(ptr); }
};

using WindowPtr = std::unique_ptr<SDL_Window, SdlDeleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SdlDeleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, SdlDeleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, SdlDeleter>;
using AudioStreamPtr = std::unique_ptr<SDL_AudioStream, SdlDeleter>;

} // namespace core