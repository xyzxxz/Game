#include "menu.hpp"

namespace ui {

Menu::Menu() = default;

void Menu::set_state(State state) noexcept { state_ = state; }
Menu::State Menu::state() const noexcept { return state_; }

void Menu::update(float /*dt*/) {}
void Menu::render(SDL_Renderer* /*renderer*/) {}

bool Menu::handle_event(const SDL_Event& /*event*/) {
    return false;
}

void Menu::on_start_game(std::function<void()> callback) {
    start_callback_ = std::move(callback);
}

void Menu::on_quit(std::function<void()> callback) {
    quit_callback_ = std::move(callback);
}

void Menu::set_final_score(int score) noexcept { final_score_ = score; }

} // namespace ui