#include "Audio.h"
#include <SDL3_mixer/SDL_mixer.h>

Audio::Audio()
    : m_mixer(nullptr)
    , m_wingAudio(nullptr)
    , m_hitAudio(nullptr)
    , m_dieAudio(nullptr)
    , m_pointAudio(nullptr) {
}

Audio::~Audio() {
    if (m_wingAudio)  MIX_DestroyAudio(m_wingAudio);
    if (m_hitAudio)   MIX_DestroyAudio(m_hitAudio);
    if (m_dieAudio)   MIX_DestroyAudio(m_dieAudio);
    if (m_pointAudio) MIX_DestroyAudio(m_pointAudio);
    if (m_mixer)      MIX_DestroyMixer(m_mixer);
}

bool Audio::Init() {
    if (!MIX_Init()) {
        SDL_Log("MIX_Init failed: %s", SDL_GetError());
        return false;
    }

    m_mixer = MIX_CreateMixer(nullptr);
    if (!m_mixer) {
        SDL_Log("MIX_CreateMixer failed: %s", SDL_GetError());
        return false;
    }

    m_wingAudio  = MIX_LoadAudio(m_mixer, "assets/audio/wing.mp3", true);
    m_hitAudio   = MIX_LoadAudio(m_mixer, "assets/audio/hit.mp3", true);
    m_dieAudio   = MIX_LoadAudio(m_mixer, "assets/audio/die.mp3", true);
    m_pointAudio = MIX_LoadAudio(m_mixer, "assets/audio/point.mp3", true);

    return true;
}

void Audio::PlayWing() {
    if (m_wingAudio) MIX_PlayAudio(m_mixer, m_wingAudio);
}

void Audio::PlayHit() {
    if (m_hitAudio) MIX_PlayAudio(m_mixer, m_hitAudio);
}

void Audio::PlayDie() {
    if (m_dieAudio) MIX_PlayAudio(m_mixer, m_dieAudio);
}

void Audio::PlayPoint() {
    if (m_pointAudio) MIX_PlayAudio(m_mixer, m_pointAudio);
}