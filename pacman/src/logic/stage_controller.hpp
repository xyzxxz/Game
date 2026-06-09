#pragma once

#include <vector>
#include "../model/types.hpp"
#include "../model/stage.hpp"
#include "../model/score.hpp"

enum class StageEvent { Continue, LifeLost, StageClear };

class GhostAI;
class Collision;

class StageController {
public:
    StageController(const StageData& stage, Score& score,
                    GhostAI& ai, Collision& collision);

    // 每帧更新，返回事件
    StageEvent update(float dt, Direction inputDir);

    const Player&     getPlayer() const { return m_player; }
    const Ghost*      getGhosts() const { return m_ghosts; }
    const StageData&  getStage()  const { return m_stage; }
    const std::vector<Pos>& getBeans() const { return m_beans; }

private:
    StageData       m_stage;
    Player          m_player;
    Ghost           m_ghosts[4];
    Score&          m_score;
    GhostAI&        m_ai;
    Collision&      m_collision;
    float           m_timer = 0.0f;
    std::vector<Pos> m_beans;
};
