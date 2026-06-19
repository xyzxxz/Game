#pragma once

#include "core/game.hpp"
#include "game/player.hpp"
#include "game/enemy.hpp"
#include "game/friendly.hpp"
#include "game/bullet.hpp"
#include "game/fuel.hpp"
#include "game/planet.hpp"
#include "game/star.hpp"
#include "ui/hud.hpp"
#include "ui/menu.hpp"
#include <random>
#include <vector>

enum class GameState { kPlaying, kPaused, kGameOver };

/// @brief 飞船射击游戏主逻辑
class StarBattleGame : public core::Game {
public:
    using core::Game::Game;

protected:
    void on_init() override;
    void on_event(const SDL_Event& event) override;
    void on_update(float dt) override;
    void on_render() override;
    void on_cleanup() override;

private:
    void spawn_entities(float dt);
    void check_collisions();
    void clean_dead_entities();
    void render_hud(SDL_Renderer* renderer);
    void render_game_over(SDL_Renderer* renderer);

    // 纹理（RAII 管理）
    std::vector<core::TexturePtr> textures_;

    // 纹理裸指针（非拥有）
    SDL_Texture* tex_bg_{};
    SDL_Texture* tex_player_{};
    SDL_Texture* tex_enemy_{};
    SDL_Texture* tex_friendly_{};
    SDL_Texture* tex_bullet_p_{};
    SDL_Texture* tex_bullet_e_{};
    SDL_Texture* tex_fuel_{};
    std::vector<SDL_Texture*> tex_planets_;
    SDL_Texture* tex_score_{};
    SDL_Texture* tex_time_{};
    SDL_Texture* tex_boom_{};
    SDL_Texture* tex_pause_{};
    SDL_Texture* tex_play_{};
    SDL_Texture* tex_mute_{};
    SDL_Texture* tex_speaker_{};
    SDL_Texture* tex_logo_{};

    // 游戏状态
    GameState state_{GameState::kPlaying};
    int score_{0};
    bool muted_{false};
    float shoot_cooldown_{0.0f};

    // 随机数
    std::mt19937 rng_;

    // 实体
    game::Player player_;
    std::vector<game::Enemy> enemies_;
    std::vector<game::Friendly> friendlies_;
    std::vector<game::Bullet> bullets_;
    std::vector<game::Fuel> fuels_;
    std::vector<game::Planet> planets_;
    game::StarManager star_manager_;

    // UI
    ui::Hud hud_;
    ui::Menu menu_;

    // 生成计时器
    float enemy_spawn_timer_{0.0f};
    float fuel_spawn_timer_{0.0f};
    float planet_spawn_timer_{0.0f};
    float friendly_spawn_timer_{0.0f};

    static constexpr float kEnemySpawnInterval = 2.0f;
    static constexpr float kFuelSpawnInterval = 3.0f;
    static constexpr float kPlanetSpawnInterval = 5.0f;
    static constexpr float kFriendlySpawnInterval = 4.0f;
};