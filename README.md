# Game

基于 C++20/23 + SDL3 的经典小游戏合集。

## 项目列表

| 项目 | 说明 |
|------|------|
| [2048](2048/) | 2048 数字合并游戏，C++23 模块化三层架构（game / renderer / app），game 层无 SDL 依赖可独立测试 |
| [Bird](Bird/) | Flappy Bird 飞翔的小鸟，C++20 + 回调式主循环，状态机（Menu → Playing → GameOver），含音效与奖牌系统 |
| [Snake](snake/) | 经典贪吃蛇，单文件实现（~260 行），蛇身加速 + 120 FPS 渲染 + 实时帧率显示 |
| [Tetris](tetris/) | 经典俄罗斯方块，含幽灵方块预览、计分系统与等级加速机制 |
| [Minesweeper](minesweeper/) | 经典扫雷，含难度选择（初级/中级/高级）与状态机模式 |
| [Sudoku](sudoku/) | 经典数独，含输入交互与场景管理 |
| [Star Battle](star-battle/) | 飞船射击游戏，C++20 + 模块化分包架构，燃料系统 + 多种敌人（敌方/友方/行星）+ RAII 资源管理 |

## 共同依赖

- **编译器**：支持 C++20 或 C++23（MSVC / GCC / Clang）
- **CMake** >= 3.20
- **[SDL3](https://github.com/libsdl-org/SDL)** >= 3.4

### 各项目额外依赖

| 项目 | SDL3_ttf | SDL3_image | SDL3_mixer | GLM | 备注 |
|------|:---:|:---:|:---:|:---:|------|
| 2048 | ✓ | | | 可选 | C++23 模块（需 MSVC） |
| Bird | | ✓ | ✓ | | C++20，回调式主循环 |
| Snake | ✓ | | | ✓ | C++23，单文件 |
| Tetris | ✓ | | | | C++23 |
| Minesweeper | | | | | C++，状态机架构 |
| Sudoku | ✓ | | | | C++ |
| Star Battle | | ✓ | ✓ | | C++20，RAII 资源管理 |

## 构建

各项目的 `CMakeLists.txt` 中需按本地环境修改 SDL 相关路径（预编译库默认位于 `D:\Library\`），然后：

```bash
cd <项目目录>
cmake -B build
cmake --build build --config Release
```

## 项目结构

```
Game/
├── 2048/            # 2048 — C++23 模块，game 层无 SDL 依赖
├── Bird/            # Flappy Bird — 状态机 + 音效
├── snake/           # 贪吃蛇 — 单文件快速上手
├── tetris/          # 俄罗斯方块 — 幽灵方块预览 + 计分
├── minesweeper/     # 扫雷 — 难度选择 + 状态机
├── sudoku/          # 数独 — 场景管理
├── star-battle/     # 飞船射击 — 燃料系统 + 多敌人类型
├── .gitignore
└── README.md
```