#pragma once

#include "sdl_deleter.hpp"
#include <string_view>

namespace core {

inline constexpr int kDefaultWindowWidth = 800;
inline constexpr int kDefaultWindowHeight = 600;

class Window {
public:
    explicit Window(std::string_view title, int width, int height,
                    SDL_WindowFlags flags = 0);

    // 禁止拷贝，允许移动
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;

    ~Window() = default;

    [[nodiscard]] SDL_Window* handle() const noexcept;

    void get_size_in_pixels(int* w, int* h) const noexcept;

    [[nodiscard]] int width() const noexcept;
    [[nodiscard]] int height() const noexcept;

private:
    WindowPtr m_handle;
    int m_width{kDefaultWindowWidth};
    int m_height{kDefaultWindowHeight};
};

} // namespace core