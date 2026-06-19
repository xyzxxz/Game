# Star Battle

基于 C++20 和 SDL3 的经典飞船射击游戏。

## 目录结构

```
star-battle/
├── src/
│   ├── core/            # 核心系统（窗口、渲染、音频、游戏循环）
│   ├── game/            # 游戏实体（玩家、敌人、友方、子弹、燃料瓶、行星、星星）
│   ├── ui/              # UI 模块（HUD、菜单）
│   ├── main.cpp         # 入口
│   ├── pch.hpp          # 预编译头
│   └── star_battle_game.{cpp,hpp}  # 主游戏逻辑
├── img/                 # 纹理图片资源
├── sound/               # 音频资源
├── CMakeLists.txt
└── README.md
```

## 构建

### 依赖

- **CMake** >= 3.20
- **C++20** 编译器（MSVC 或 Clang/GCC）
- **SDL3** (3.4.10) + SDL3_image + SDL3_mixer

预编译库路径配置在 `CMakeLists.txt` 中：

```cmake
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3")
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3_image")
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3_mixer")
```

如你的库路径不同，请修改以上路径或通过 `-DCMAKE_PREFIX_PATH` 传入。

### 编译

```powershell
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

生成的可执行文件为 `build/Debug/star-battle.exe`。

## 操作说明

| 按键 | 功能 |
|------|------|
| W/A/S/D | 移动飞船 |
| Space | 发射子弹 |
| P | 暂停 / 继续 |
| M | 静音 / 取消静音 |

## 游戏规则

- **燃料系统**：初始 15 点，每秒 -1。归零则游戏结束。
- **燃料瓶**：触碰 +15 燃料（上限 30）。
- **敌方飞船**：子弹击中 +5 分；碰撞 -15 燃料。
- **行星**：需 2 次击中摧毁，+10 分；碰撞 -10 燃料。
- **友方飞船**：子弹击中 -10 分；碰撞 -10 分。
- **分数**：允许负分。

## 技术栈

- C++20
- SDL3 渲染与音频
- SDL3_image 纹理加载
- SDL3_mixer 背景音乐与音效
- RAII 资源管理（`std::unique_ptr` + 自定义 SdlDeleter）
- 模块化分包架构