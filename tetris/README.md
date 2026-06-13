# 俄罗斯方块 (Tetris)

基于 **SDL3** + **SDL3_ttf** 的经典俄罗斯方块游戏。

## 游戏规则

- 10×20 的棋盘，七种标准方块（I、O、T、S、Z、J、L）随机下落
- 填满一行即可消除，得分随消除行数和等级递增
- 每消除 10 行等级提升一级，下落速度加快
- 方块堆到顶部时游戏结束

## 操作说明

| 按键 | 功能 |
|------|------|
| A / D | 左右移动 |
| S     | 加速下落 |
| W     | 顺时针旋转 |
| 空格   | 硬降（直接落底） |
| R     | 游戏结束后重新开始 |

## 界面布局

- **左侧面板**：显示得分、等级、消行数、下一个方块预览
- **右侧区域**：10×20 的棋盘，含半透明幽灵方块预览落点

## 构建与运行

### 依赖

- [SDL3](https://github.com/libsdl-org/SDL) (>= 3.4)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)
- CMake (>= 3.10)
- 支持 C++23 的编译器

### 构建步骤

1. 修改 `CMakeLists.txt` 中的 `SDL3_DIR` 和 `SDL3_ttf_DIR` 路径为你的本地安装路径
2. 执行构建：

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

3. 运行 `tetris.exe`

## 项目结构

```
tetris/
├── src/
│   ├── main.cpp         # 程序入口、游戏主循环、输入处理
│   ├── types.hpp        # 公共类型、常量和枚举定义
│   ├── tetromino.hpp    # 方块形状数据接口
│   ├── tetromino.cpp    # 七种标准方块的形状与旋转定义
│   ├── board.hpp        # 棋盘管理接口
│   ├── board.cpp        # 10×20 网格、碰撞检测、消行
│   ├── game.hpp         # 游戏核心逻辑接口
│   ├── game.cpp         # 状态管理、计分升级、方块生成
│   ├── renderer.hpp     # 渲染模块接口
│   └── renderer.cpp     # SDL3 绘制（棋盘、方块、面板、文字）
├── CMakeLists.txt
└── README.md
```