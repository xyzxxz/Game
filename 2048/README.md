# 2048

基于 C++23 模块 + SDL3 实现的 2048 小游戏。

## 构建

### 依赖

- **编译器**：MSVC 19.x（支持 C++23 模块）
- **CMake** >= 3.28
- **SDL3**：放置于 `D:/Library/SDL3-3.4.10/`
- **SDL3_ttf**：放置于 `D:/Library/SDL3_ttf/`
- **GLM**（可选）：放置于 `D:/Library/glm/`

### 编译

```powershell
mkdir build
cmake -B build
cmake --build build --config Release
```

## 项目结构

```
2048/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp          # 入口：创建 App 并调用 run()
    ├── game.ixx          # game 模块接口 — 纯逻辑（方向 棋盘 Game）
    ├── game.cpp          # game 模块实现
    ├── renderer.ixx      # renderer 模块接口 — 渲染（色板 棋盘 菜单 HUD）
    ├── renderer.cpp      # renderer 模块实现
    ├── app.ixx           # app 模块接口 — 应用生命周期（窗口 主循环 事件）
    └── app.cpp           # app 模块实现
```

## 架构

```
main.cpp
  └─ import app
       ├─ import game        # 方向 状态 棋盘 Game（不依赖 SDL）
       └─ import renderer    # Colors BoardRenderer MenuRenderer HUDRenderer
            └─ import game
```

| 模块 | 职责 |
|------|------|
| `game` | 纯游戏逻辑：`Direction` `GameState` `Tile` `Board` `Game`，无 SDL 依赖 |
| `renderer` | 渲染层：主题色板定义，棋盘/菜单/HUD 的 `SDL_Renderer` 绘制 |
| `app` | 应用层：窗口创建、主循环、事件分发、按 `GameState` 路由渲染 |

## 密钥点

- **game 模块不依赖 SDL**，可脱离图形环境单独测试
- **main.cpp 仅 7 行**，责任单一
- 三个渲染器各司其职：`BoardRenderer` 绘制棋盘，`MenuRenderer` 绘制菜单，`HUDRenderer` 绘制分数信息
