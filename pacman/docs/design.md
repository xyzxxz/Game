# 吃豆人 (Pacman) 游戏架构设计

## 概述

- **渲染引擎**: SDL3 + SDL3_image
- **帧率**: 120 FPS
- **场景**: 主菜单 → 关卡选择 → 游戏关卡 → 暂停 → 结算
- **关卡**: 共 9 关，每关不同地图，难度（幽灵速度）递增
- **地图格式**: 二维整数数组（0=Wall, 1=Bean, 2=PowerBean, 3=Empty, 4=GhostHouse）
- **幽灵 AI**: 统一 BFS 最短路径追踪
- **架构风格**: MVC 分层 + 状态机

---

## 一、文件结构

```
src/
├── main.cpp                      # 入口
├── game.hpp / game.cpp           # 主循环 + 状态机 + 场景调度
├── input.hpp                     # 键盘输入管理
├── resource.hpp / resource.cpp   # 图片加载、缓存、按 key 获取纹理
│
├── model/                        # 纯数据层，不依赖 SDL
│   ├── types.hpp                 # 共用枚举、方向、坐标结构体
│   ├── map.hpp                   # 地图数据
│   ├── maps.hpp                  # 9 个关卡的地图硬编码数据
│   ├── player.hpp                # 玩家数据
│   ├── ghost.hpp                 # 幽灵数据
│   ├── stage.hpp                 # 单个关卡完整数据集
│   └── score.hpp                 # 分数 / 生命 / 关卡
│
├── view/                         # 渲染层，持有 SDL_Texture
│   ├── menu_renderer.hpp
│   ├── level_select_renderer.hpp
│   ├── stage_renderer.hpp
│   └── hud_renderer.hpp
│
└── logic/                        # 游戏逻辑
    ├── collision.hpp             # 碰撞检测
    ├── ghost_ai.hpp              # 幽灵 AI
    └── stage_controller.hpp      # 单关卡的帧更新逻辑
```

---

## 二、Model 层（纯数据）

### `model/types.hpp` — 共用枚举与结构体

| 类型 | 值 | 说明 |
|---|---|---|
| `Direction` | `Up, Down, Left, Right, None` | 移动方向 |
| `TileType` | `Wall, Bean, PowerBean, Empty, GhostHouse` | 地图格子类型 |
| `GameState` | `Menu, LevelSelect, Playing, Paused, LifeLost, StageClear, GameOver, GameWin` | 游戏顶层状态 |
| `GhostState` | `Chase, Scatter, Frightened, Eaten` | 幽灵行为状态 |
| `Pos` | `{ int x, y }` | 网格坐标 |

### `model/map.hpp`

```
Map {
    int      width, height;           // 格子列数、行数
    int      tileSize;                // 每个格子像素尺寸
    TileType grid[height][width];     // 二维网格

    TileType at(int x, int y) const;
    bool     isWalkable(int x, int y) const;  // != Wall
}
```

### `model/maps.hpp`

9 个关卡的 `static constexpr` 二维整数数组（`LEVEL_1` ~ `LEVEL_9`），编译期嵌入。每个整数对应 `TileType`。

### `model/player.hpp`

```
Player {
    Pos       pos;            // 当前网格位置（浮点子像素）
    Direction dir;            // 当前移动方向
    Direction nextDir;        // 缓存下一方向（输入后等待路口可用）
    float     speed;          // 格/秒
    int       lives;          // 剩余生命
    bool      invincible;     // 受伤后短暂无敌
}
```

### `model/ghost.hpp`

```
Ghost {
    int        id;            // 0~3
    Pos        pos;           // 位置
    Direction  dir;           // 方向
    float      speed;         // 速度（随关卡递增）
    GhostState state;         // 当前行为状态
    Pos        homePos;       // 出生点（鬼屋）
}
```

### `model/stage.hpp`

```
StageData {
    int       levelNum;           // 1~9
    Map       map;
    Pos       playerStart;
    Ghost     ghosts[4];
    int       totalBeans;         // 豆子总数
    float     ghostBaseSpeed;     // 幽灵基础速度 × 难度系数
    int       powerBeanCount;     // 能量豆数量
}
```

### `model/score.hpp`

```
Score {
    int score;         // 总分
    int lives;         // 剩余生命（初始 3）
    int level;         // 当前关卡 1~9
    int beansEaten;    // 本关已吃豆子数
    int combo;         // 连续吃幽灵连击
}
```

---

## 三、资源管理

### `resource.hpp`

```
ResourceManager {
    SDL_Renderer* renderer;
    unordered_map<string, SDL_Texture*> cache;

    SDL_Texture* getTexture(const string& key);
    void         loadAll();    // 启动时一次性加载全部图片
}
```

**纹理 key 命名规范：**

| Key | 用途 |
|---|---|
| `wall` | 墙 |
| `bean` | 普通豆子 |
| `power_bean` | 大能量豆 |
| `empty` | 空地 |
| `player_up/down/left/right` | 玩家各朝向 |
| `ghost_0` ~ `ghost_3` | 4 只幽灵 |
| `ghost_frightened` | 受惊幽灵 |
| `ghost_eaten` | 被吃幽灵眼睛 |
| `menu_bg` | 主菜单背景 |
| `menu_start_btn` | 开始按钮 |
| `menu_exit_btn` | 退出按钮 |
| `level_select_bg` | 选关背景 |
| `level_btn_1` ~ `level_btn_9` | 关卡按钮 |
| `level_btn_locked` | 未解锁关卡 |
| `hud_life` | 生命图标 |
| `pause_overlay` | 暂停遮罩 |
| `gameover_bg` | 游戏结束背景 |
| `gamewin_bg` | 通关背景 |

---

## 四、View 层（渲染）

所有 Renderer 接收 `SDL_Renderer*` 和 `ResourceManager&`，从 Model 读取数据并绘制对应纹理。

### `view/menu_renderer.hpp`

```
MenuRenderer {
    void render(SDL_Renderer* r, ResourceManager& res);
    int  selectedItem();     // 当前高亮项: 0=开始, 1=退出
}
```
- 背景图 + "开始游戏"/"退出" 两个选项，键盘 ↑↓ 切换高亮

### `view/level_select_renderer.hpp`

```
LevelSelectRenderer {
    void render(SDL_Renderer* r, ResourceManager& res,
                int unlockedLevel);
    int  selectedLevel();    // 当前选中关卡
}
```
- 1~9 关按钮网格排列，未解锁关卡灰色不可选

### `view/stage_renderer.hpp`

```
StageRenderer {
    void render(SDL_Renderer* r, ResourceManager& res,
                const StageData& stage,
                const Player& player,
                const Ghost ghosts[4]);
}
```
- **地图**: 逐格遍历 `Map.grid`，根据 `TileType` 取纹理，计算像素坐标绘制
- **豆子**: 已被吃的豆子不再绘制
- **玩家**: 根据 `player.dir` 选朝向纹理，画在 pixel 位置（含平滑子像素）
- **幽灵**: 根据 `ghost.state` 选纹理，画在 pixel 位置
- 绘制顺序：地图 → 豆子 → 幽灵 → 玩家（确保玩家在最上层）

### `view/hud_renderer.hpp`

```
HudRenderer {
    void render(SDL_Renderer* r, ResourceManager& res,
                const Score& score);
}
```
- 画面顶部显示：分数（数字）、生命图标（×N）、当前关卡号

---

## 五、Logic 层

### `logic/ghost_ai.hpp`

```
GhostAI {
    Direction decide(const Ghost& ghost, const Player& player,
                     const Map& map);
}
```

- **Chase**: BFS 以玩家位置为目标，返回最短路径第一步方向
- **Scatter**: 目标改为地图四角之一（4 幽灵各不同角）
- **Frightened**: 随机选可走方向
- **Eaten**: 目标为鬼屋入口，快速返回
- 模式切换由 `StageController` 按时间周期控制（Chase ↔ Scatter 循环）

### `logic/collision.hpp`

```
Collision {
    int  checkBean(const Player& player,
                   const vector<Pos>& beans,
                   const Map& map);
    bool checkPlayerGhost(const Player& player, const Ghost& ghost);
    bool canTurn(const Pos& pos, Direction dir, const Map& map);
    bool isWall(const Pos& pos, const Map& map);
}
```

- `checkBean`: 判断玩家是否与豆子同格，返回被吃豆子索引（-1 = 无）
- `checkPlayerGhost`: 判断玩家与幽灵碰撞（同格或交叉经过）
- `canTurn`: 检查该格能否向指定方向行走（即目标格不是墙）
- `isWall`: 检查指定格子是否为墙

### `logic/stage_controller.hpp`

```
StageController {
    StageData       stage;
    Player          player;
    Ghost           ghosts[4];
    Score&          score;
    float           timer;
    vector<Pos>     beans;       // 本关剩余豆子

    void update(float dt, Direction inputDir);
}
```

**每帧 update 流程：**

1. 处理玩家输入 → 设 `player.nextDir`
2. 移动玩家：路口处可切换方向，否则保持原方向；若 `canTurn` 为 false 则停止
3. GhostAI 决定每个幽灵方向 → 移动所有幽灵
4. `checkBean()` → 吃到豆子：删除 beans、加分（普通 10 分，能量豆 50 分）；吃到能量豆：所有幽灵进入 Frightened
5. `checkPlayerGhost()` → Frightened 碰撞：幽灵变 Eaten、加分（200×combo）、combo+1；普通碰撞：玩家死亡
6. 检查 `beans.empty()` → 通关
7. 返回状态提示：`Continue / LifeLost / StageClear`

---

## 六、输入管理

### `input.hpp`

```
InputManager {
    Direction getDirection();     // WASD / 方向键
    bool      isPausePressed();   // ESC / P
    bool      isConfirmPressed(); // Enter / Space
    bool      isBackPressed();    // Backspace
    bool      isUpPressed();      // ↑ / W
    bool      isDownPressed();    // ↓ / S
}
```

---

## 七、Game 主循环

### `game.hpp`

```
Game {
    SDL_Window*    window;
    SDL_Renderer*  renderer;
    GameState      state;
    int            unlockedLevel;    // 已解锁最高关卡 1~9
    bool           running;

    ResourceManager    resource;
    InputManager       input;
    MenuRenderer       menuRenderer;
    LevelSelectRenderer  levelSelectRenderer;
    StageRenderer      stageRenderer;
    HudRenderer        hudRenderer;
    GhostAI            ghostAI;
    Collision          collision;

    Score              score;
    StageController*   stageCtrl;     // 当前关卡
}
```

### 主循环流程

```
run() {
    while (running) {
        float dt = capFPS(120);
        handleEvents();

        switch (state) {
        case Menu:
            updateMenu(dt);
            menuRenderer.render(r, resource);
            break;
        case LevelSelect:
            updateLevelSelect(dt);
            levelSelectRenderer.render(r, resource, unlockedLevel);
            break;
        case Playing:
            updatePlaying(dt);
            stageRenderer.render(...);
            hudRenderer.render(r, resource, score);
            break;
        case Paused:
            updatePaused(dt);
            // 覆盖半透明遮罩 + "暂停"文字
            break;
        case LifeLost:
            // 短暂动画后: 有命→Playing, 无命→GameOver
            break;
        case StageClear:
            // 短暂动画后: 非9关→LevelSelect, 9关→GameWin
            break;
        case GameOver:
            // 显示结算, 确认→Menu
            break;
        case GameWin:
            // 显示通关, 确认→Menu
            break;
        }
        SDL_RenderPresent(renderer);
    }
}
```

### 状态流转图

```
Menu ──开始──▶ LevelSelect ──选关──▶ Playing
                                        │
                        ┌──────暂停──────┤
                        ▼               │
                      Paused            │
                        │               ▼
                        └────继续──▶ Playing
                                        │
                              ┌─吃光豆子─┤
                              ▼         ▼
                        StageClear   LifeLost
                              │         │
                    非9关 ▼  9关▼    有命▼  无命▼
                   LevelSelect GameWin Playing GameOver
                                                 │
                        Menu ◀───────────────────┘
                   GameWin ───────────────────→ Menu
```

---

## 八、构建配置

`CMakeLists.txt` 中 `add_executable` 需包含所有 `.cpp` 文件：

```
add_executable(pacman
    src/main.cpp
    src/game.cpp
    src/resource.cpp
    # 后续 logic/ view/ 下的 .cpp 按需加入
)
```

依赖：SDL3 + SDL3_image，C++23 标准。

---

## 九、各关卡难度参数

| 关卡 | 幽灵速度系数 | 地图复杂度 |
|---|---|---|
| 1 | 0.75× | 简单 |
| 2 | 0.80× | — |
| 3 | 0.85× | — |
| 4 | 0.90× | — |
| 5 | 1.00× | 中等 |
| 6 | 1.05× | — |
| 7 | 1.10× | — |
| 8 | 1.15× | — |
| 9 | 1.25× | 复杂 |

- 玩家速度保持恒定
- 幽灵 Chase/Scatter 模式切换周期随关卡缩短

---

## 十、图片资源清单

| 分类 | 所需图片 |
|---|---|
| 地图 | 墙、豆子、能量豆、空地 |
| 玩家 | 上/下/左/右 四朝向各一张 |
| 幽灵 | 4 只幽灵各一张、受惊状态一张、眼睛（被吃）一张 |
| 菜单 | 背景图、开始按钮（普通+高亮）、退出按钮（普通+高亮） |
| 选关 | 背景图、关卡按钮 1~9、锁定按钮 |
| HUD | 生命图标 |
| 通用 | 暂停遮罩、游戏结束背景、通关背景 |
