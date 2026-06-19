#include "star_battle_game.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <vector>

namespace {

inline constexpr float kPlayerShootCooldown = 0.25f;
inline constexpr int kEnemyScore = 5;
inline constexpr int kPlanetScore = 10;
inline constexpr int kFriendlyPenalty = 10;
inline constexpr int kEnemyCollisionFuelLoss = 15;

inline bool check_collision(const SDL_FRect& a, const SDL_FRect& b) {
    return SDL_HasRectIntersectionFloat(&a, &b);
}

} // anonymous namespace

void StarBattleGame::on_init() {
    SDL_SetAppMetadata("Star Battle", "1.0.0", "com.starbattle.game");

    // 加载纹理（裸指针，生命周期由 StarBattleGame 成员管理）
    auto load = [&](const char* path) -> SDL_Texture* {
        auto tex = renderer().load_texture(path);
        if (!tex) {
            SDL_Log("Failed to load texture: %s", path);
        }
        auto* ptr = tex.release();
        textures_.push_back(core::TexturePtr{ptr});
        return ptr;
    };

    tex_bg_       = load("img/background-1.jpg");
    tex_player_   = load("img/plane/player.png");
    tex_enemy_    = load("img/plane/enemy.png");
    tex_friendly_ = load("img/plane/friend.png");
    tex_bullet_p_ = load("img/playerBullet.png");
    tex_bullet_e_ = load("img/enemyBullet.png");
    tex_fuel_     = load("img/fuel2.png");

    tex_planets_.push_back(load("img/meteorites/meteorites_1.png"));
    tex_planets_.push_back(load("img/meteorites/meteorites_2.png"));
    tex_planets_.push_back(load("img/meteorites/meteorites_3.png"));
    tex_planets_.push_back(load("img/meteorites/meteorites_4.png"));

    tex_score_    = load("img/score.png");
    tex_time_     = load("img/time3.png");
    tex_boom_     = load("img/boom.png");
    tex_pause_    = load("img/pause.png");
    tex_play_     = load("img/play.png");
    tex_mute_     = load("img/mute.png");
    tex_speaker_  = load("img/speaker.png");
    tex_logo_     = load("img/logo-01.png");

    // 初始化星星管理器
    star_manager_ = game::StarManager{
        static_cast<float>(window().width()),
        static_cast<float>(window().height())};

    // 随机数
    rng_.seed(std::random_device{}());

    // 播放背景音乐
    audio().play_music("sound/background.mp3");

    state_ = GameState::kPlaying;
}

void StarBattleGame::on_event(const SDL_Event& event) {
    if (event.type != SDL_EVENT_KEY_DOWN) return;

    auto key = event.key.scancode;

    if (state_ == GameState::kGameOver) {
        if (key == SDL_SCANCODE_RETURN || key == SDL_SCANCODE_SPACE) {
            // 重新开始
            score_ = 0;
            player_ = game::Player{};
            enemies_.clear();
            friendlies_.clear();
            bullets_.clear();
            fuels_.clear();
            planets_.clear();
            state_ = GameState::kPlaying;
        }
        return;
    }

    if (key == SDL_SCANCODE_P) {
        state_ = (state_ == GameState::kPlaying)
                     ? GameState::kPaused
                     : GameState::kPlaying;
        return;
    }

    if (key == SDL_SCANCODE_M) {
        muted_ = !muted_;
        audio().set_muted(muted_);
        return;
    }

    if (state_ != GameState::kPlaying) return;

    if (key == SDL_SCANCODE_SPACE && shoot_cooldown_ <= 0.0f) {
        // 发射子弹（从玩家中心向上）
        bullets_.emplace_back(player_.x(), player_.y() - 30.0f,
                              game::BulletType::kPlayer, 0.0f, -1.0f);
        shoot_cooldown_ = kPlayerShootCooldown;
        audio().play_sfx("sound/shoot.mp3");
    }
}

void StarBattleGame::on_update(float dt) {
    if (state_ != GameState::kPlaying) return;

    shoot_cooldown_ = std::max(0.0f, shoot_cooldown_ - dt);

    // 键盘输入（WASD）
    int num_keys = 0;
    const bool* keys = SDL_GetKeyboardState(&num_keys);
    float dx = 0.0f;
    float dy = 0.0f;
    if (keys[SDL_SCANCODE_A]) dx -= 1.0f;
    if (keys[SDL_SCANCODE_D]) dx += 1.0f;
    if (keys[SDL_SCANCODE_W]) dy -= 1.0f;
    if (keys[SDL_SCANCODE_S]) dy += 1.0f;

    // 归一化斜向移动
    if (dx != 0.0f && dy != 0.0f) {
        float inv = 1.0f / std::sqrt(2.0f);
        dx *= inv;
        dy *= inv;
    }
    player_.move(dx * dt, dy * dt);

    player_.update(dt);

    // 生成实体
    spawn_entities(dt);

    // 更新实体
    star_manager_.update(dt);
    for (auto& e : enemies_) e.update(dt);
    for (auto& f : friendlies_) f.update(dt);
    for (auto& b : bullets_) b.update(dt);
    for (auto& fl : fuels_) fl.update(dt);
    for (auto& p : planets_) p.update(dt);

    // 碰撞检测
    check_collisions();

    // 清理无效实体
    clean_dead_entities();

    // 检查游戏结束
    if (!player_.is_alive()) {
        state_ = GameState::kGameOver;
        audio().play_sfx("sound/destroyed.mp3");
    }

    // 更新 HUD
    hud_.update(score_, player_.fuel(), state_ == GameState::kPaused, muted_);
}

void StarBattleGame::on_render() {
    auto* r = renderer().handle();

    renderer().set_draw_color(10, 10, 30, 255);
    renderer().clear();

    // 背景图
    if (tex_bg_) {
        SDL_FRect bg_dst{0, 0, static_cast<float>(window().width()),
                          static_cast<float>(window().height())};
        SDL_RenderTexture(r, tex_bg_, nullptr, &bg_dst);
    }

    // 星星
    star_manager_.render(r);

    if (state_ == GameState::kGameOver) {
        // 游戏结束画面
        render_game_over(r);
    } else {
        // 渲染实体
        for (auto& e : enemies_) e.render(r, tex_enemy_);
        for (auto& f : friendlies_) f.render(r, tex_friendly_);
        for (auto& fl : fuels_) fl.render(r, tex_fuel_);

        for (auto& p : planets_) {
            // 根据 HP 选择纹理：满血用随机纹理，受伤用固定纹理
            int idx = std::min(static_cast<int>(planets_.size()), 3);
            SDL_Texture* tex = tex_planets_[idx % tex_planets_.size()];
            p.render(r, tex);
        }

        for (auto& b : bullets_) {
            SDL_Texture* tex = (b.type() == game::BulletType::kPlayer)
                                   ? tex_bullet_p_
                                   : tex_bullet_e_;
            b.render(r, tex);
        }

        player_.render(r, tex_player_);

        // HUD
        render_hud(r);
    }

    renderer().present();
}

void StarBattleGame::on_cleanup() {
    // 纹理由 textures_ vector 自动释放
}

void StarBattleGame::spawn_entities(float dt) {
    float screen_w = static_cast<float>(window().width());

    std::uniform_real_distribution<float> x_dist(
        game::Enemy::kWidth, screen_w - game::Enemy::kWidth);

    // 敌方飞船
    enemy_spawn_timer_ += dt;
    while (enemy_spawn_timer_ >= kEnemySpawnInterval) {
        enemy_spawn_timer_ -= kEnemySpawnInterval;
        enemies_.emplace_back(x_dist(rng_), -game::Enemy::kHeight);
    }

    // 燃料瓶
    fuel_spawn_timer_ += dt;
    while (fuel_spawn_timer_ >= kFuelSpawnInterval) {
        fuel_spawn_timer_ -= kFuelSpawnInterval;
        fuels_.emplace_back(x_dist(rng_), -game::Fuel::kHeight);
    }

    // 行星
    planet_spawn_timer_ += dt;
    while (planet_spawn_timer_ >= kPlanetSpawnInterval) {
        planet_spawn_timer_ -= kPlanetSpawnInterval;
        planets_.emplace_back(x_dist(rng_), -game::Planet::kHeight);
    }

    // 友方飞船
    friendly_spawn_timer_ += dt;
    while (friendly_spawn_timer_ >= kFriendlySpawnInterval) {
        friendly_spawn_timer_ -= kFriendlySpawnInterval;
        friendlies_.emplace_back(x_dist(rng_), -game::Friendly::kHeight);
    }
}

void StarBattleGame::check_collisions() {
    SDL_FRect player_rect = player_.rect();

    // 玩家子弹 命中 敌方
    for (auto& bullet : bullets_) {
        if (!bullet.is_active() || bullet.type() != game::BulletType::kPlayer)
            continue;

        SDL_FRect bullet_rect = bullet.rect();

        // 命中敌方
        for (auto& enemy : enemies_) {
            if (!enemy.is_alive()) continue;
            if (check_collision(bullet_rect, enemy.rect())) {
                enemy.destroy();
                bullet.deactivate();
                score_ += kEnemyScore;
                break;
            }
        }

        if (!bullet.is_active()) continue;

        // 命中友方（扣分）
        for (auto& friendly : friendlies_) {
            if (!friendly.is_alive()) continue;
            if (check_collision(bullet_rect, friendly.rect())) {
                friendly.destroy();
                bullet.deactivate();
                score_ -= kFriendlyPenalty;
                break;
            }
        }

        if (!bullet.is_active()) continue;

        // 命中行星
        for (auto& planet : planets_) {
            if (!planet.is_alive()) continue;
            if (check_collision(bullet_rect, planet.rect())) {
                bullet.deactivate();
                if (planet.hit()) {
                    score_ += kPlanetScore;
                }
                break;
            }
        }
    }

    // 敌方子弹 命中 玩家（这里简化：敌方暂不发射子弹，仅通过碰撞造成伤害）

    // 玩家 碰撞 敌方（减燃料）
    for (auto& enemy : enemies_) {
        if (!enemy.is_alive()) continue;
        if (check_collision(player_rect, enemy.rect())) {
            enemy.destroy();
            player_.deduct_fuel(kEnemyCollisionFuelLoss);
        }
    }

    // 玩家 碰撞 友方（扣分）
    for (auto& friendly : friendlies_) {
        if (!friendly.is_alive()) continue;
        if (check_collision(player_rect, friendly.rect())) {
            friendly.destroy();
            score_ -= kFriendlyPenalty;
        }
    }

    // 玩家 碰撞 燃料瓶
    for (auto& fuel : fuels_) {
        if (fuel.is_collected()) continue;
        if (check_collision(player_rect, fuel.rect())) {
            fuel.collect();
            player_.add_fuel(game::Fuel::kFuelAmount);
        }
    }

    // 玩家 碰撞 行星
    for (auto& planet : planets_) {
        if (!planet.is_alive()) continue;
        if (check_collision(player_rect, planet.rect())) {
            planet.hit();
            player_.deduct_fuel(10);
        }
    }
}

void StarBattleGame::clean_dead_entities() {
    float screen_h = static_cast<float>(window().height());

    std::erase_if(enemies_, [screen_h](const game::Enemy& e) {
        return !e.is_alive() || e.y() > screen_h + 100.0f;
    });
    std::erase_if(friendlies_, [screen_h](const game::Friendly& f) {
        return !f.is_alive() || f.y() > screen_h + 100.0f;
    });
    std::erase_if(bullets_, [](const game::Bullet& b) {
        return !b.is_active() || b.is_off_screen(800.0f, 600.0f);
    });
    std::erase_if(fuels_, [screen_h](const game::Fuel& f) {
        return f.is_collected() || f.is_off_screen(screen_h);
    });
    std::erase_if(planets_, [screen_h](const game::Planet& p) {
        return !p.is_alive() || p.y() > screen_h + 100.0f;
    });
}

void StarBattleGame::render_hud(SDL_Renderer* renderer) {
    // 分数图标 + 数值（简化：仅显示图标位置）
    SDL_FRect score_icon{10, 10, 24, 24};
    SDL_RenderTexture(renderer, tex_score_, nullptr, &score_icon);

    // 燃料图标
    SDL_FRect fuel_icon{10, 44, 24, 24};
    SDL_RenderTexture(renderer, tex_fuel_, nullptr, &fuel_icon);

    // 暂停/播放图标
    SDL_FRect pause_icon{750, 10, 32, 32};
    SDL_Texture* ptex = (state_ == GameState::kPaused) ? tex_play_ : tex_pause_;
    SDL_RenderTexture(renderer, ptex, nullptr, &pause_icon);

    // 静音图标
    SDL_FRect mute_icon{750, 52, 32, 32};
    SDL_Texture* mtex = muted_ ? tex_mute_ : tex_speaker_;
    SDL_RenderTexture(renderer, mtex, nullptr, &mute_icon);
}

void StarBattleGame::render_game_over(SDL_Renderer* renderer) {
    // 半透明黑色遮罩
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_FRect overlay{0, 0, static_cast<float>(window().width()),
                      static_cast<float>(window().height())};
    SDL_RenderFillRect(renderer, &overlay);

    // Logo
    if (tex_logo_) {
        SDL_FRect logo_dst{200, 150, 400, 100};
        SDL_RenderTexture(renderer, tex_logo_, nullptr, &logo_dst);
    }
}