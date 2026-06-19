#include "audio.hpp"
#include <SDL3/SDL_audio.h>
#include <stdexcept>

namespace core {

AudioManager::AudioManager()
{
    if (!MIX_Init()) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (dev == 0) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_AudioSpec spec;
    int sample_frames;
    SDL_GetAudioDeviceFormat(dev, &spec, &sample_frames);

    mixer_ = MIX_CreateMixerDevice(dev, &spec);
    if (!mixer_) {
        SDL_CloseAudioDevice(dev);
        throw std::runtime_error(SDL_GetError());
    }
    device_id_ = dev;
}

AudioManager::~AudioManager()
{
    if (music_track_) {
        MIX_DestroyTrack(music_track_);
    }
    for (auto* track : sfx_tracks_) {
        MIX_DestroyTrack(track);
    }
    sfx_tracks_.clear();

    if (mixer_) {
        MIX_DestroyMixer(mixer_);
    }
    if (device_id_ != 0) {
        SDL_CloseAudioDevice(device_id_);
    }
    MIX_Quit();
}

void AudioManager::play_music(std::string_view path)
{
    if (m_muted) return;

    if (music_track_) {
        MIX_DestroyTrack(music_track_);
        music_track_ = nullptr;
    }

    auto* audio = MIX_LoadAudio(mixer_, path.data(), true);
    if (!audio) {
        SDL_Log("MIX_LoadAudio(%s) failed: %s", path.data(), SDL_GetError());
        return;
    }

    music_track_ = MIX_CreateTrack(mixer_);
    MIX_SetTrackAudio(music_track_, audio);

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(music_track_, props);
    SDL_DestroyProperties(props);
}

void AudioManager::play_sfx(std::string_view path)
{
    if (m_muted) return;

    auto* audio = MIX_LoadAudio(mixer_, path.data(), false);
    if (!audio) {
        SDL_Log("MIX_LoadAudio(%s) failed: %s", path.data(), SDL_GetError());
        return;
    }

    auto* track = MIX_CreateTrack(mixer_);
    MIX_SetTrackAudio(track, audio);

    SDL_PropertiesID props = SDL_CreateProperties();
    MIX_PlayTrack(track, props);
    SDL_DestroyProperties(props);

    sfx_tracks_.push_back(track);
}

void AudioManager::set_muted(bool muted) noexcept
{
    m_muted = muted;
    if (muted) {
        MIX_PauseAllTracks(mixer_);
    } else {
        MIX_ResumeAllTracks(mixer_);
    }
}

bool AudioManager::is_muted() const noexcept { return m_muted; }

void AudioManager::set_music_volume(float volume) noexcept
{
    m_music_volume = volume;
    MIX_SetMixerGain(mixer_, volume);
}

void AudioManager::set_sfx_volume(float volume) noexcept
{
    m_sfx_volume = volume;
}

} // namespace core