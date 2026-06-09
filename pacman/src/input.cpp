#include "input.hpp"

void InputManager::update() {
    // TODO: 读取键盘状态
    m_keyState = SDL_GetKeyboardState(nullptr);
}

Direction InputManager::getDirection() const {
    if (isUpPressed())    return Direction::Up;
    if (isDownPressed())  return Direction::Down;
    if (isLeftPressed())  return Direction::Left;
    if (isRightPressed()) return Direction::Right;
    return Direction::None;
}

bool InputManager::isPausePressed() const {
    // TODO
    return false;
}

bool InputManager::isConfirmPressed() const {
    // TODO
    return false;
}

bool InputManager::isBackPressed() const {
    // TODO
    return false;
}

bool InputManager::isUpPressed() const {
    // TODO
    return false;
}

bool InputManager::isDownPressed() const {
    // TODO
    return false;
}

bool InputManager::isLeftPressed() const {
    // TODO
    return false;
}

bool InputManager::isRightPressed() const {
    // TODO
    return false;
}

bool InputManager::isQuitRequested() const {
    return m_quit;
}
