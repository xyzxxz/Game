# Flappy Bird (SDL3)

基于 **C++20** + **SDL3** 实现的飞翔的小鸟游戏，采用回调式主循环架构。

## 依赖

| 库 | 说明 |
|---|---|
| [SDL3](https://github.com/libsdl-org/SDL) 3.4.10 | 核心框架（窗口、渲染、输入、音频） |
| SDL3_image | PNG 图片加载 |
| SDL3_mixer | MP3 音效播放 |

预编译库位于 `D:\Library\`，CMake 通过 `find_package` 直接链接。

## 构建

```bash
cmake -B build -S .
cmake --build build
```

构建产物（`Bird.exe`、`SDL3.dll`、`SDL3_image.dll`、`SDL3_mixer.dll`、`assets/`）均输出到 `build/` 目录。

## 运行

```bash
cd build
.\Bird.exe
```

> 从 `build/` 目录启动游戏，确保相对路径 `assets/` 可被正确访问。

## 操作

| 按键 | 功能 |
|---|---|
| 空格 / 鼠标左键 | 开始游戏 / 小鸟飞翔 / 重新开始 |
| P | 暂停 / 恢复 |
| ESC | 退出游戏 |

## 项目结构

```
Bird/
├── CMakeLists.txt
├── assets/
│   ├── audio/          # wing.mp3, hit.mp3, die.mp3, point.mp3
│   ├── buttons/        # start, pause, resume, ok, share, score, menu
│   └── sprites/        # 小鸟、管道、背景、地面、数字、奖牌、Logo
├── src/
│   ├── main.cpp        # SDL3 回调入口（SDL_AppInit/Event/Iterate/Quit）
│   ├── Game.h/cpp      # 游戏状态机（Menu → Playing → GameOver）
│   ├── Config.h        # 游戏常量
│   ├── Bird.h/cpp      # 小鸟物理、动画、渲染
│   ├── Pipe.h/cpp      # 管道生成、移动、碰撞
│   ├── Ground.h/cpp    # 滚动地面
│   ├── Background.h/cpp# 滚动背景
│   ├── Score.h/cpp     # 计分、最高分、奖牌
│   ├── Button.h/cpp    # 通用按钮
│   ├── UI.h/cpp        # 菜单、HUD、结算面板
│   └── Audio.h/cpp     # 音效管理
└── README.md
```