#include "window.hpp"
#include <stdexcept>

namespace core {

Window::Window(std::string_view title, int width, int height, SDL_WindowFlags flags)
    : m_width{width}
    , m_height{height}
{
    m_handle.reset(SDL_CreateWindow(title.data(), width, height, flags));
    if (!m_handle) {
        throw std::runtime_error(SDL_GetError());
    }
}

SDL_Window* Window::handle() const noexcept { return m_handle.get(); }

void Window::get_size_in_pixels(int* w, int* h) const noexcept
{
    SDL_GetWindowSizeInPixels(m_handle.get(), w, h);
}

int Window::width() const noexcept { return m_width; }
int Window::height() const noexcept { return m_height; }

} // namespace core