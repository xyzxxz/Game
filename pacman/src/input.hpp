#pragma once

#include <SDL3/SDL.h>
#include "model/types.hpp"

class InputManager {
public:
    void      update();              // 每帧调用，读取键盘状态
    Direction getDirection() const;
    bool      isPausePressed() const;
    bool      isConfirmPressed() const;
    bool      isBackPressed() const;
    bool      isUpPressed() const;
    bool      isDownPressed() const;
    bool      isLeftPressed() const;
    bool      isRightPressed() const;
    bool      isQuitRequested() const;

private:
    const Uint8* m_keyState = nullptr;
    bool         m_quit     = false;
};
