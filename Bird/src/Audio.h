#pragma once

struct MIX_Mixer;
struct MIX_Audio;

class Audio {
public:
    Audio();
    ~Audio();

    bool Init();

    void PlayWing();
    void PlayHit();
    void PlayDie();
    void PlayPoint();

private:
    MIX_Mixer* m_mixer;
    MIX_Audio* m_wingAudio;
    MIX_Audio* m_hitAudio;
    MIX_Audio* m_dieAudio;
    MIX_Audio* m_pointAudio;
};