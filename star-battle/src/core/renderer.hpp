#pragma once

#include "sdl_deleter.hpp"
#include <SDL3/SDL.h>
#include <string_view>

namespace core {

class Renderer {
public:
    explicit Renderer(SDL_Window* window);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = default;
    Renderer& operator=(Renderer&&) noexcept = default;

    ~Renderer() = default;

    [[nodiscard]] SDL_Renderer* handle() const noexcept;

    /// @brief 加载纹理，返回 nullptr 表示失败
    [[nodiscard]] TexturePtr load_texture(std::string_view path);

    void set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) noexcept;

    void clear() noexcept;

    void present() noexcept;

private:
    RendererPtr m_handle;
};

} // namespace core