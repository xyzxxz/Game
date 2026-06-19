#pragma once

#include <SDL3/SDL.h>
#include <vector>

struct PipePair {
    float x;
    float gapY;
    bool  scored;
};

class PipeManager {
public:
    PipeManager();
    ~PipeManager();

    bool Init(SDL_Renderer* renderer);
    void Reset();
    void Update(float dt);
    void Render(SDL_Renderer* renderer);

    std::vector<PipePair>& GetPipes();
    bool CheckCollision(float birdX, float birdY, float birdW, float birdH) const;

private:
    std::vector<PipePair> m_pipes;
    float                 m_spawnTimer;
    SDL_Texture*          m_texture;
};