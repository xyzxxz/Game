# Game

基于 C++23 + SDL3 的经典小游戏合集。

## 项目列表

| 项目 | 说明 |
|------|------|
| [2048](2048/) | 2048 数字合并游戏，game / renderer / app 三层架构 |
| [Snake](snake/) | 经典贪吃蛇，单文件实现（~260 行） |
| [Tetris](tetris/) | 经典俄罗斯方块，含幽灵方块预览与计分系统 |
| [Minesweeper](minesweeper/) | 经典扫雷，含难度选择与状态机模式 |
| [Sudoku](sudoku/) | 经典数独 |

## 共同依赖

- **编译器**：支持 C++23（MSVC / GCC / Clang）
- **CMake** >= 3.20
- **[SDL3](https://github.com/libsdl-org/SDL)** >= 3.4
- **[SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)**（文字渲染）

## 构建

各项目的 `CMakeLists.txt` 中需按本地环境修改 SDL 相关路径，然后：

```bash
cd <项目目录>
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## 项目结构

```
Game/
├── 2048/          # 2048 — 三层架构（game / renderer / app）
├── snake/         # 贪吃蛇 — 单文件快速上手
├── tetris/        # 俄罗斯方块 — 经典头文件分层
├── minesweeper/   # 扫雷 — 难度 + 状态机
├── sudoku/        # 数独
├── .gitignore
└── README.md
```