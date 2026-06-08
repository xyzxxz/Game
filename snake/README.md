# Snake

基于 SDL3 的经典贪吃蛇游戏，单文件实现（~260 行）。

## 操作

| 按键 | 功能 |
|------|------|
| `W` `A` `S` `D` | 控制方向（禁止反向） |
| `Space` | 暂停 / 继续 |

## 特性

- 蛇身加速：吃得越多移动越快（200ms → 50ms）
- 120 FPS 渲染，网格背景
- 顶部 HUD：分数 + 实时帧率
- Game Over 保留最终分数

## 依赖

- **SDL3** ≥ 3.4（图形+输入+计时）
- **SDL3_ttf**（文字渲染）
- **glm**（二维坐标）
- **CMake** ≥ 3.10
- C++23 编译器（MSVC / GCC / Clang）

## 构建

```bash
# 修改 CMakeLists.txt 中的依赖路径
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

构建后 SDL DLL 自动复制到 exe 目录。

## 运行

```bash
# 在项目根目录放置 font.ttf 字体文件
./build/Debug/snake.exe
```

> 需要 `font.ttf` 文件放在 exe 同级目录，用于显示文字。无字体时游戏仍可运行，仅不显示文字。
