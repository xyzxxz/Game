#include "stage_controller.hpp"
#include "ghost_ai.hpp"
#include "collision.hpp"

StageController::StageController(const StageData& stage, Score& score,
                                  GhostAI& ai, Collision& collision)
    : m_stage(stage), m_score(score), m_ai(ai), m_collision(collision) {
    // TODO: 初始化玩家和幽灵位置
}

StageEvent StageController::update(float dt, Direction inputDir) {
    // TODO: 逐帧更新逻辑
    (void)dt;
    (void)inputDir;
    return StageEvent::Continue;
}
