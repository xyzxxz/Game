#pragma once

#include "sdl_deleter.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace core {

class AudioManager {
public:
    AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) noexcept = default;
    AudioManager& operator=(AudioManager&&) noexcept = default;

    ~AudioManager();

    /// @brief 播放背景音乐
    void play_music(std::string_view path);

    /// @brief 播放音效
    void play_sfx(std::string_view path);

    void set_muted(bool muted) noexcept;
    [[nodiscard]] bool is_muted() const noexcept;

    void set_music_volume(float volume) noexcept;
    void set_sfx_volume(float volume) noexcept;

private:
    MIX_Mixer* mixer_{};
    SDL_AudioDeviceID device_id_{};
    MIX_Track* music_track_{};
    std::vector<MIX_Track*> sfx_tracks_;
    std::unordered_map<std::string, MIX_Track*> tracks_;

    bool m_muted{false};
    float m_music_volume{1.0f};
    float m_sfx_volume{1.0f};
};

} // namespace core