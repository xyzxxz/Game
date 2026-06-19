#include "Pipe.h"
#include <SDL3_image/SDL_image.h>
#include "Config.h"
#include <cstdlib>

PipeManager::PipeManager()
    : m_spawnTimer(0.0f)
    , m_texture(nullptr) {
}

PipeManager::~PipeManager() {
    if (m_texture) SDL_DestroyTexture(m_texture);
}

bool PipeManager::Init(SDL_Renderer* renderer) {
    m_texture = IMG_LoadTexture(renderer, "assets/sprites/pipe-green.png");
    return m_texture != nullptr;
}

void PipeManager::Reset() {
    m_pipes.clear();
    m_spawnTimer = 0.0f;
}

void PipeManager::Update(float dt) {
    // 移动已有管道
    for (auto it = m_pipes.begin(); it != m_pipes.end();) {
        it->x -= kPipeSpeed * dt;

        // 移除移出屏幕的管道
        if (it->x + kPipeWidth < 0) {
            it = m_pipes.erase(it);
        } else {
            ++it;
        }
    }

    // 生成新管道
    m_spawnTimer += dt;
    if (m_spawnTimer >= kPipeSpawnInterval) {
        m_spawnTimer -= kPipeSpawnInterval;

        PipePair pipe;
        pipe.x = kScreenWidth;
        pipe.gapY = 150.0f + static_cast<float>(rand() % 200);  // 150 ~ 350
        pipe.scored = false;
        m_pipes.push_back(pipe);
    }
}

void PipeManager::Render(SDL_Renderer* renderer) {
    // pipe-green.png 尺寸: 52 x 320 (上半部分翻转绘制)
    constexpr float pipeHeight = 320.0f;

    for (const auto& pipe : m_pipes) {
        // 上管道：翻转绘制
        float topPipeBottom = pipe.gapY - kPipeGap / 2.0f;
        SDL_FRect topSrc = {0, pipeHeight - topPipeBottom, kPipeWidth, topPipeBottom};
        SDL_FRect topDst = {pipe.x, 0, kPipeWidth, topPipeBottom};
        SDL_RenderTexture(renderer, m_texture, &topSrc, &topDst);

        // 下管道
        float bottomPipeTop = pipe.gapY + kPipeGap / 2.0f;
        float bottomPipeHeight = kScreenHeight - kGroundHeight - bottomPipeTop;
        if (bottomPipeHeight > 0) {
            SDL_FRect bottomSrc = {0, 0, kPipeWidth, bottomPipeHeight};
            SDL_FRect bottomDst = {pipe.x, bottomPipeTop, kPipeWidth, bottomPipeHeight};
            SDL_RenderTexture(renderer, m_texture, &bottomSrc, &bottomDst);
        }
    }
}

std::vector<PipePair>& PipeManager::GetPipes() {
    return m_pipes;
}

bool PipeManager::CheckCollision(float birdX, float birdY, float birdW, float birdH) const {
    for (const auto& pipe : m_pipes) {
        // 上管道碰撞
        float topBottom = pipe.gapY - kPipeGap / 2.0f;
        if (birdY < topBottom &&
            birdX + birdW > pipe.x && birdX < pipe.x + kPipeWidth) {
            return true;
        }

        // 下管道碰撞
        float bottomTop = pipe.gapY + kPipeGap / 2.0f;
        if (birdY + birdH > bottomTop &&
            birdX + birdW > pipe.x && birdX < pipe.x + kPipeWidth) {
            return true;
        }
    }
    return false;
}