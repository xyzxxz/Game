---
name: SDL3-dev-standards
description: C++ 项目中 SDL3 的开发规范和最佳实践指南。当用户询问关于 SDL3 的 C++ 项目结构、CMake 构建、RAII 资源管理、初始化流程、事件循环、渲染管线（2D/GPU）、音频系统、错误处理、性能优化、或从 SDL2 迁移到 SDL3 的问题时，必须使用此 skill。也适用于用户开始新的 C++ SDL3 项目、审查 SDL3 代码、或排查 SDL3 相关问题时的规范参考。
---

# C++ SDL3 开发规范

本规范定义了在 **C++ 项目**中使用 SDL3 进行开发时应遵循的编码标准、项目结构和最佳实践。遵循这些规范可以确保代码的可维护性、可移植性和性能。

---

## 一、项目结构规范

### 1.1 推荐的 C++ 项目目录结构

```
my-sdl3-project/
├── assets/                    # 游戏资源文件（图片、音频、字体等）
│   ├── images/
│   ├── audio/
│   └── fonts/
├── src/                       # C++ 源代码目录
│   ├── main.cpp               # 程序入口点
│   ├── core/                  # 核心系统
│   │   ├── Window.hpp/cpp
│   │   ├── Renderer.hpp/cpp
│   │   └── AudioSystem.hpp/cpp
│   ├── game/                  # 游戏逻辑
│   │   ├── Player.hpp/cpp
│   │   ├── World.hpp/cpp
│   │   └── entities/
│   ├── ui/                    # 用户界面
│   │   ├── Menu.hpp/cpp
│   │   └── HUD.hpp/cpp
│   └── utils/                 # 工具函数
│       ├── Math.hpp/cpp
│       └── FileIO.hpp/cpp
├── shaders/                   # GPU 着色器源码
│   ├── vertex/
│   └── fragment/
├── external/                  # 第三方依赖（git submodules 或 vendored）
│   └── SDL/                   # SDL3 源码子模块
├── build/                     # 构建输出目录
├── CMakeLists.txt             # 根 CMake 构建文件
├── vcpkg.json                 # vcpkg 依赖清单（可选）
└── README.md
```

### 1.2 源文件组织原则

- **一个类一个文件对**：每个 C++ 类对应一个 `.hpp`（或 `.h`）头文件和一个 `.cpp` 实现文件
- **头文件职责明确**：头文件只包含声明和模板定义，实现放在 `.cpp` 中
- **main 文件独立**：`main()` / Main Callbacks 放在单独的 `main.cpp` 中，不与其他逻辑混合
- **按命名空间分包**：使用 C++ `namespace` 组织模块，与目录结构对应

---

## 二、构建系统规范

### 2.1 CMake 集成

推荐使用 CMake 作为 C++ 项目的构建系统。有以下几种 SDL3 集成方式：

#### 方式一：使用本地预编译 SDL3（Windows 推荐）

Windows 上使用预编译好的 SDL3 库（位于 `D:\Library\SDL3`，含 `cmake` 配置目录）：

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定 SDL3 预编译库路径
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3")

# 寻找 SDL3（会从 D:/Library/SDL3/cmake 中加载配置）
find_package(SDL3 REQUIRED CONFIG)

add_executable(mygame WIN32 src/main.cpp)
target_link_libraries(mygame PRIVATE SDL3::SDL3)

# 扩展库
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3_image")
find_package(SDL3_image REQUIRED CONFIG)
target_link_libraries(mygame PRIVATE SDL3_image::SDL3_image)
```

也可通过 CMake 命令行参数指定（推荐，避免硬编码路径）：

```bash
cmake -B build -DCMAKE_PREFIX_PATH="D:/Library/SDL3;D:/Library/SDL3_image"
```

#### 方式二：使用系统安装的 SDL3（Linux/macOS）

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

# 使用 C++17 或更高版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 寻找 SDL3
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

# 创建可执行文件
add_executable(mygame WIN32
    src/main.cpp
    src/core/Window.cpp
    src/core/Renderer.cpp
    src/game/Player.cpp
)

# 链接 SDL3
target_link_libraries(mygame PRIVATE SDL3::SDL3)

# 扩展库（可选）
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
target_link_libraries(mygame PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

# 启用 LSP 支持（clangd）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

#### 方式二：使用 vcpkg 管理依赖（推荐用于团队协作）

```cmake
# vcpkg.json
{
  "name": "my-game",
  "version": "1.0.0",
  "dependencies": [
    "sdl3",
    "sdl3-image",
    "sdl3-ttf"
  ]
}

# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 使用 vcpkg toolchain 时自动处理 find_package
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

add_executable(mygame WIN32 src/main.cpp)
target_link_libraries(mygame PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
```

### 2.2 CMake 构建类型

| 构建类型 | 用途 | 优化级别 |
|---------|------|---------|
| `Debug` | 开发调试，包含符号信息和断言 | `-Og` + 无优化 |
| `Release` | 发布版本，最高优化 | `-O3` |
| `RelWithDebInfo` | 带调试信息的优化版本 | `-O2 -g` |
| `MinSizeRel` | 最小体积发布 | `-Os` |

### 2.3 SDL3 自身 CMake 选项（作为子模块时）

| 选项 | 说明 |
|------|------|
| `-DSDL_SHARED=ON` | 构建动态库（默认） |
| `-DSDL_STATIC=ON` | 构建静态库 |
| `-DSDL_TESTS=ON` | 构建测试程序 |

---

## 三、C++ 编码规范

### 3.1 头文件包含规范

```cpp
// SDL3 核心头文件 —— C++ 项目中同样只需包含这一个（推荐）
#include <SDL3/SDL.h>

// main 入口需要额外包含（仅在一个 .cpp 中包含）
#include <SDL3/SDL_main.h>

// 扩展库头文件
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
```

**包含顺序规范：**
1. 对应模块的头文件（`.cpp` 优先包含自己的 `.hpp`）
2. C++ 标准库头文件
3. SDL3 头文件
4. 项目内其他头文件
5. 第三方库头文件

**若使用 PCH（预编译头）：**
```cpp
// pch.hpp
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <expected>   // C++23
#include <span>
#include <chrono>
#include <filesystem>
```

### 3.2 命名规范

| 元素 | 规范 | 示例 |
|------|------|------|
| 类/结构体 | PascalCase | `class GameWindow`, `struct PlayerData` |
| 成员变量 | m_ 前缀 + snake_case | `m_window_width`, `m_frame_count` |
| 成员函数 | PascalCase 或 snake_case（项目内统一） | `CreateWindow()` 或 `create_window()` |
| 函数参数 | snake_case | `window_title`, `init_flags` |
| 常量 | k 前缀 + PascalCase 或 UPPER_SNAKE | `kMaxPlayers` 或 `MAX_PLAYERS` |
| 命名空间 | 全小写 | `namespace core`, `namespace game` |
| 枚举 | `enum class` | `enum class BlendMode { Alpha, Additive }` |
| SDL3 类型 | 保持 SDL 原始命名 | `SDL_Window*`, `SDL_Texture*` |
| 模板参数 | 大写字母 | `template <typename T>` |

### 3.3 C++ 代码风格示例

```cpp
#include <SDL3/SDL.h>
#include <memory>
#include <string_view>
#include <expected>  // C++23

namespace core {

// 常量的两种风格选择一种即可
inline constexpr int kWindowWidth = 800;
inline constexpr int kWindowHeight = 600;

class Window {
public:
    /// @brief 使用字符串视图避免不必要的 std::string 构造
    explicit Window(std::string_view title, int width, int height, SDL_WindowFlags flags = 0)
        : m_handle{SDL_CreateWindow(title.data(), width, height, flags)}
    {
        if (!m_handle) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    // 禁止拷贝
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // 允许移动
    Window(Window&& other) noexcept : m_handle{std::exchange(other.m_handle, nullptr)} {}
    Window& operator=(Window&& other) noexcept
    {
        if (this != &other) {
            Destroy();
            m_handle = std::exchange(other.m_handle, nullptr);
        }
        return *this;
    }

    ~Window() { Destroy(); }

    // 访问器
    [[nodiscard]] SDL_Window* Handle() const noexcept { return m_handle; }

    /// @brief 使用 SDL3 的像素尺寸（HiDPI 感知）
    void GetSizeInPixels(int* w, int* h) const noexcept
    {
        SDL_GetWindowSizeInPixels(m_handle, w, h);
    }

private:
    void Destroy() noexcept
    {
        if (m_handle) {
            SDL_DestroyWindow(m_handle);
            m_handle = nullptr;
        }
    }

    SDL_Window* m_handle = nullptr;
};

} // namespace core
```

---

## 四、RAII 资源管理（核心规范）

在 C++ 项目中使用 SDL3，**必须使用 RAII（Resource Acquisition Is Initialization）模式**管理所有 SDL 资源的生命周期。

### 4.1 基于 std::unique_ptr 的方案（推荐）

```cpp
namespace core {

// SDL3 资源删除器 —— 为 unique_ptr 提供自定义删除
struct SdlDeleter {
    void operator()(SDL_Window* ptr) const noexcept   { SDL_DestroyWindow(ptr); }
    void operator()(SDL_Renderer* ptr) const noexcept { SDL_DestroyRenderer(ptr); }
    void operator()(SDL_Texture* ptr) const noexcept  { SDL_DestroyTexture(ptr); }
    void operator()(SDL_Surface* ptr) const noexcept  { SDL_DestroySurface(ptr); }
    void operator()(SDL_AudioStream* ptr) const noexcept { SDL_DestroyAudioStream(ptr); }
    void operator()(SDL_GPUDevice* ptr) const noexcept { SDL_DestroyGPUDevice(ptr); }
    void operator()(SDL_GPUShader* ptr) const noexcept { SDL_ReleaseGPUShader(ptr); }
    void operator()(SDL_GPUBuffer* ptr) const noexcept { SDL_ReleaseGPUBuffer(ptr); }
    void operator()(SDL_GPUTexture* ptr) const noexcept { SDL_ReleaseGPUTexture(ptr); }
    void operator()(SDL_GPUGraphicsPipeline* ptr) const noexcept { SDL_ReleaseGPUGraphicsPipeline(ptr); }
};

// 类型别名
using WindowPtr      = std::unique_ptr<SDL_Window, SdlDeleter>;
using RendererPtr    = std::unique_ptr<SDL_Renderer, SdlDeleter>;
using TexturePtr     = std::unique_ptr<SDL_Texture, SdlDeleter>;
using SurfacePtr     = std::unique_ptr<SDL_Surface, SdlDeleter>;
using AudioStreamPtr = std::unique_ptr<SDL_AudioStream, SdlDeleter>;

// 工厂函数 —— 返回 unique_ptr
[[nodiscard]] inline WindowPtr CreateWindow(
    std::string_view title, int w, int h, SDL_WindowFlags flags)
{
    auto* handle = SDL_CreateWindow(title.data(), w, h, flags);
    if (!handle) throw std::runtime_error(SDL_GetError());
    return WindowPtr{handle};
}

// 若项目不使用异常，可返回 std::optional
[[nodiscard]] inline std::optional<WindowPtr> CreateWindowSafe(
    std::string_view title, int w, int h, SDL_WindowFlags flags) noexcept
{
    auto* handle = SDL_CreateWindow(title.data(), w, h, flags);
    if (!handle) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        return std::nullopt;
    }
    return WindowPtr{handle};
}

} // namespace core
```

### 4.2 使用示例

```cpp
#include "core/SdlResources.hpp"

class Game {
public:
    Game(std::string_view title, int w, int h)
        : m_window{core::CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE)}
        , m_renderer{core::CreateRenderer(m_window.get())}
    {
        // 资源已在创建时通过 unique_ptr 管理，无需手动释放
    }

    Game(Game&&) = default;
    Game& operator=(Game&&) = default;

private:
    core::WindowPtr   m_window;
    core::RendererPtr m_renderer;
    std::vector<core::TexturePtr> m_textures;
};

// 使用方无需调用任何 Destroy* 函数
```

---

## 五、入口点与主循环

### 5.1 传统 main() 方式（桌面端推荐）

```cpp
// main.cpp —— 仅此文件包含 SDL_main.h
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/Game.hpp"
#include <cstdlib>

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    auto game = std::make_unique<Game>("My Game", 800, 600);
    game->Run();

    return EXIT_SUCCESS;
}
```

**重要注意事项：**
- `SDL_main.h` **只能在一个 `.cpp` 文件中包含**
- `SDL_main.h` 会 `#define main` 为 `SDL_main`，不要在 C++ 代码中使用 `main` 作变量名
- `SDL.h` **不包含** `SDL_main.h`，必须显式包含

### 5.2 Main Callbacks 方式（跨平台：移动端/Web 推荐）

```cpp
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/SdlResources.hpp"

struct GameState {
    core::WindowPtr   window;
    core::RendererPtr renderer;
    Uint64            last_time = 0;
};

int SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    auto state = std::make_unique<GameState>();
    state->window   = core::CreateWindow("SDL3 Callbacks", 800, 600, 0);
    state->renderer = core::CreateRenderer(state->window.get());
    state->last_time = SDL_GetTicks();

    *appstate = state.release();
    return 0;
}

int SDL_AppEvent(void* appstate, const SDL_Event* event)
{
    auto& state = *static_cast<GameState*>(appstate);
    if (event->type == SDL_EVENT_QUIT) return 1;
    return 0;
}

int SDL_AppIterate(void* appstate)
{
    auto& state = *static_cast<GameState*>(appstate);

    Uint64 now = SDL_GetTicks();
    float delta_time = (now - state.last_time) / 1000.0f;
    state.last_time = now;

    SDL_SetRenderDrawColor(state.renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(state.renderer.get());
    // ... 绘制 ...
    SDL_RenderPresent(state.renderer.get());

    return 0;
}

void SDL_AppQuit(void* appstate, int result)
{
    delete static_cast<GameState*>(appstate);
}
```

**决策原则：**
- **仅桌面**：传统 `main()` + `unique_ptr` RAII
- **跨平台（移动端/Web）**：Main Callbacks
- **状态管理**：`struct` 打包 + `void*` 传递

### 5.3 主循环结构

```
每帧循环:
  1. 处理事件（SDL_PollEvent 或 SDL_AppEvent）
  2. 更新游戏状态（基于 delta_time）
  3. 清空渲染目标
  4. 绘制场景
  5. 呈现（SDL_RenderPresent 或提交 GPU 命令缓冲区）
```

---

## 六、初始化与清理规范

### 6.1 子系统初始化

```cpp
// 返回 bool（true=成功）
if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
}
```

**与 SDL2 的变化：**
- `SDL_Init()` 返回 `bool` 而非 `int`（负值=错误 → false=失败）
- `SDL_AudioInit()` / `SDL_AudioQuit()` 已移除，改用 `SDL_InitSubSystem()` / `SDL_QuitSubSystem()`
- 子系统使用引用计数管理

### 6.2 窗口创建

```cpp
// SDL3 移除了 x, y 参数
auto window = core::CreateWindow("My Game", 800, 600,
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

// HiDPI 感知
int pixel_w = 0, pixel_h = 0;
SDL_GetWindowSizeInPixels(window.get(), &pixel_w, &pixel_h);
```

### 6.3 RAII 意味着无需手动清理

使用 `std::unique_ptr` 管理 SDL 资源后，**不需要显式调用 `SDL_Destroy*` 函数**。资源在 `unique_ptr` 析构时自动释放。

---

## 七、事件处理规范

### 7.1 SDL3 事件命名变化

| SDL2 | SDL3 |
|------|------|
| `SDL_QUIT` | `SDL_EVENT_QUIT` |
| `SDL_KEYDOWN` | `SDL_EVENT_KEY_DOWN` |
| `SDL_KEYUP` | `SDL_EVENT_KEY_UP` |
| `SDL_MOUSEMOTION` | `SDL_EVENT_MOUSE_MOTION` |
| `SDL_MOUSEBUTTONDOWN` | `SDL_EVENT_MOUSE_BUTTON_DOWN` |

### 7.2 事件循环 C++ 写法

```cpp
void Game::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            m_running = false;
            break;

        case SDL_EVENT_KEY_DOWN:
            HandleKeyPress(event.key);
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            OnWindowResized();
            break;

        default:
            break;
        }
    }
}
```

**键盘输入最佳实践：**
- 使用 `SDL_EVENT_KEY_DOWN` / `SDL_EVENT_KEY_UP` 检测按键
- 使用 `SDL_EVENT_TEXT_INPUT` 处理文本输入
- 使用 `event.key.repeat` 字段区分首次按下和重复
- 实时键盘状态：`SDL_GetKeyboardState(nullptr)` 获取 `const bool*` 数组

---

## 八、2D 渲染规范

### 8.1 渲染器初始化

```cpp
class Renderer2D {
public:
    explicit Renderer2D(SDL_Window* window)
        : m_renderer{core::CreateRenderer(window)}
    {
        SDL_SetRenderLogicalPresentation(
            m_renderer.get(), 1920, 1080,
            SDL_LOGICAL_PRESENTATION_LETTERBOX);
    }

    [[nodiscard]] SDL_Renderer* Handle() const noexcept
    {
        return m_renderer.get();
    }

private:
    core::RendererPtr m_renderer;
};
```

### 8.2 SDL2→SDL3 渲染函数重命名

| SDL2 | SDL3 |
|------|------|
| `SDL_RenderCopy()` | `SDL_RenderTexture()` |
| `SDL_RenderCopyEx()` | `SDL_RenderTextureRotated()` |
| `SDL_RenderDrawPoint()` | `SDL_RenderPoint()` |
| `SDL_RenderDrawPoints()` | `SDL_RenderPoints()` |
| `SDL_RenderDrawLine()` | `SDL_RenderLine()` |
| `SDL_RenderDrawLines()` | `SDL_RenderLines()` |
| `SDL_RenderDrawRect()` | `SDL_RenderRect()` |
| `SDL_RenderFillRect()` | `SDL_RenderFillRect()`（不变）|
| `SDL_RenderSetClipRect()` | `SDL_SetRenderClipRect()` |

---

## 九、GPU API 规范

### 9.1 RAII 封装

```cpp
namespace gpu {

using DevicePtr   = std::unique_ptr<SDL_GPUDevice, core::SdlDeleter>;
using ShaderPtr   = std::unique_ptr<SDL_GPUShader, core::SdlDeleter>;
using BufferPtr   = std::unique_ptr<SDL_GPUBuffer, core::SdlDeleter>;
using TexturePtr  = std::unique_ptr<SDL_GPUTexture, core::SdlDeleter>;
using PipelinePtr = std::unique_ptr<SDL_GPUGraphicsPipeline, core::SdlDeleter>;

DevicePtr CreateDevice(bool debug_mode = true)
{
    auto formats = SDL_GetGPUShaderFormats();
    auto* device = SDL_CreateGPUDevice(formats, debug_mode, "MyGame");
    if (!device) throw std::runtime_error(SDL_GetError());
    return DevicePtr{device};
}

class CommandBuffer {
public:
    explicit CommandBuffer(SDL_GPUDevice* device) noexcept
        : m_device{device}
        , m_handle{SDL_AcquireGPUCommandBuffer(device)} {}

    ~CommandBuffer()
    {
        if (m_handle && !m_submitted) {
            SDL_SubmitGPUCommandBuffer(m_handle);
        }
    }

    CommandBuffer(CommandBuffer&&) = default;
    CommandBuffer& operator=(CommandBuffer&&) = default;
    CommandBuffer(const CommandBuffer&) = delete;

    void Submit()
    {
        if (m_handle && !m_submitted) {
            SDL_SubmitGPUCommandBuffer(m_handle);
            m_submitted = true;
        }
    }

    [[nodiscard]] SDL_GPUCommandBuffer* Handle() const noexcept { return m_handle; }

private:
    SDL_GPUDevice*       m_device = nullptr;
    SDL_GPUCommandBuffer* m_handle = nullptr;
    bool                 m_submitted = false;
};

} // namespace gpu
```

### 9.2 GPU 渲染管线流程

```
初始化阶段:
  1. SDL_CreateGPUDevice()        → DevicePtr
  2. SDL_ClaimWindowForGPUDevice()
  3. SDL_CreateGPUShader()         → ShaderPtr
  4. SDL_CreateGPUGraphicsPipeline() → PipelinePtr
  5. SDL_CreateGPUBuffer()         → BufferPtr

每帧渲染:
  1. auto cmd = gpu::CommandBuffer{device}
  2. SDL_WaitAndAcquireGPUSwapchainTexture()
  3. SDL_BeginGPURenderPass()
  4. SDL_BindGPUGraphicsPipeline()
  5. SDL_BindGPUVertexBuffers()
  6. SDL_DrawGPUPrimitives()
  7. SDL_EndGPURenderPass()
  8. cmd.Submit()
```

### 9.3 GPU 性能最佳实践

- 最小化 Render Pass 数量（pass 开始/结束开销大）
- 按管线分组绘制，减少状态切换
- 尽早数据上传（帧开始时）
- 使用资源缓存，避免帧中创建/释放
- 使用 Storage Buffer 而非 Uniform Buffer 存大量数据
- 正确使用资源循环（cycling）机制

### 9.4 GPU 2D 混合渲染（SDL 3.4+）

```cpp
auto gpu_renderer = SDL_CreateGPURenderer(gpu_device, window);
// 可自由混合 2D 渲染和 GPU 底层渲染
```

---

## 十、音频系统规范

### 10.1 RAII 音频封装

```cpp
namespace audio {

class AudioPlayer {
public:
    AudioPlayer()
    {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
            throw std::runtime_error(
                std::format("Audio init failed: {}", SDL_GetError()));

        SDL_AudioSpec dst_spec;
        SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &dst_spec);

        SDL_AudioSpec src_spec;
        SDL_GetAudioDeviceFormat(dev, &src_spec);
        m_stream.reset(SDL_CreateAudioStream(&src_spec, &dst_spec));

        SDL_BindAudioStreams(dev, m_stream.get());
        SDL_ResumeAudioDevice(dev);
        m_device = dev;
    }

    void Play(const std::vector<std::byte>& data)
    {
        SDL_PutAudioStreamData(m_stream.get(), data.data(), data.size());
    }

    void SetVolume(float gain) noexcept { SDL_SetAudioStreamGain(m_stream.get(), gain); }

    ~AudioPlayer() { SDL_BindAudioStreams(m_device, nullptr); }

private:
    core::AudioStreamPtr m_stream;
    SDL_AudioDeviceID    m_device = 0;
};

} // namespace audio
```

### 10.2 音频注意事项

- `SDL_AudioStream` 内部维护锁，线程安全
- SDL3 支持默认音频设备热插拔自动迁移
- 多流混音：多个 stream 可绑定到同一设备
- SDL3 中音频回调不再是主要方式

---

## 十一、错误处理规范

### 11.1 C++ 错误处理模式

C++ 项目有两种主流错误处理策略，请统一选择一种：

#### 模式 A：异常（推荐）

```cpp
class SdlError : public std::runtime_error {
public:
    explicit SdlError(std::string_view context)
        : std::runtime_error{
              std::format("{}: {}", context, SDL_GetError())} {}
};

// 工厂函数抛异常
[[nodiscard]] inline WindowPtr CreateWindow(
    std::string_view title, int w, int h, SDL_WindowFlags flags)
{
    auto* handle = SDL_CreateWindow(title.data(), w, h, flags);
    if (!handle) throw SdlError("SDL_CreateWindow");
    return WindowPtr{handle};
}
```

#### 模式 B：std::expected（C++23，无异常项目）

```cpp
enum class SdlErr { InitFailed, WindowFailed, RendererFailed };

[[nodiscard]] inline std::expected<WindowPtr, SdlErr>
CreateWindowSafe(std::string_view title, int w, int h, SDL_WindowFlags flags)
{
    auto* handle = SDL_CreateWindow(title.data(), w, h, flags);
    if (!handle) return std::unexpected(SdlErr::WindowFailed);
    return WindowPtr{handle};
}
```

### 11.2 错误处理规则

1. **必须检查 SDL3 函数返回值**来判断是否出错
2. `SDL_GetError()` **不会在成功调用后被清除**，不能依赖其是否为空
3. 错误字符串是**线程局部**的

### 11.3 日志系统

```cpp
SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Fatal: %s", msg);
SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Warning: %s", msg);
SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Info: %s", msg);
```

---

## 十二、多线程规范

### 12.1 使用 C++ 标准线程

```cpp
#include <thread>
#include <atomic>
#include <mutex>

class Worker {
public:
    void Start()
    {
        m_running.store(true);
        m_thread = std::thread{&Worker::Loop, this};
    }

    void Stop()
    {
        m_running.store(false);
        if (m_thread.joinable()) m_thread.join();
    }

    ~Worker() { Stop(); }

private:
    void Loop()
    {
        while (m_running.load()) {
            std::lock_guard lock{m_mutex};
            // 线程安全操作
        }
    }

    std::thread         m_thread;
    std::atomic<bool>   m_running{false};
    std::mutex          m_mutex;
};
```

### 12.2 线程安全要点

| 操作 | 线程安全？ |
|------|-----------|
| SDL 渲染器/GPU 调用 | ❌ 仅在主线程 |
| `SDL_AudioStream` | ✅ 线程安全 |
| `SDL_GetError()` | ✅ 线程局部 |
| `SDL_PushEvent()` | ✅ 线程安全 |

---

## 十三、平台特定规范

### 13.1 跨平台检测

```cpp
#if defined(SDL_PLATFORM_WIN32)
    // Windows
#elif defined(SDL_PLATFORM_LINUX)
    // Linux / SteamOS
#elif defined(SDL_PLATFORM_MACOS)
    // macOS
#elif defined(SDL_PLATFORM_IOS)
    // iOS
#elif defined(SDL_PLATFORM_ANDROID)
    // Android
#elif defined(SDL_PLATFORM_EMSCRIPTEN)
    // Web (Emscripten/WASM)
#endif
```

### 13.2 应用元数据

```cpp
// 初始化时尽早调用
SDL_SetAppMetadata("My Game", "1.0.0", "com.example.mygame");
```

---

## 十四、从 SDL2 迁移到 SDL3（C++ 项目）

### 14.1 关键变化速查

| 类别 | SDL2 | SDL3 (C++ 项目) |
|------|------|------|
| 头文件 | `#include <SDL2/SDL.h>` | `#include <SDL3/SDL.h>` |
| main 入口 | SDL 自动处理 | 需 `#include <SDL3/SDL_main.h>` |
| 返回值 | `int`（负值=错误） | `bool`（true=成功） |
| 初始化检查 | `if (SDL_Init() < 0)` | `if (!SDL_Init(...))` |
| 事件 | `SDL_QUIT`, `SDL_KEYDOWN` | `SDL_EVENT_QUIT`, `SDL_EVENT_KEY_DOWN` |
| 窗口 | `CreateWindow(t, x, y, w, h, f)` | `CreateWindow(t, w, h, f)` |
| 渲染 Copy | `SDL_RenderCopy()` | `SDL_RenderTexture()` |
| 音频 | 回调函数 | `SDL_AudioStream` |
| 资源管理 | 手动 `Destroy*` | **`std::unique_ptr` + RAII** |
| GPU API | 无 | `SDL_CreateGPUDevice()` 等 |

### 14.2 C++ 项目迁移步骤

1. **运行 rename 脚本**机械替换函数名和头文件路径
2. **添加 `SDL_main.h`**：在入口文件加入显式 include
3. **替换返回值检查**：`if (func() < 0)` → `if (!func())`
4. **重命名事件常量**：`SDL_QUIT` → `SDL_EVENT_QUIT`
5. **封装 RAII 包装器**：`std::unique_ptr` + 自定义删除器
6. **重构音频**：从回调改为 `SDL_AudioStream` 推送模式
7. **更新窗口创建**：移除 `x, y` 参数

---

## 十五、性能最佳实践汇总

### 15.1 渲染优化

- **2D**：纹理图集减少切换，合并渲染调用，使用裁剪区域
- **GPU**：最少 Render Pass，按管线分组，尽早数据上传

### 15.2 RAII 零开销

`std::unique_ptr` 自定义删除器在生产构建中内联后与裸指针无异，不应为"性能"回到手动管理。

### 15.3 音频优化

- 用 `SDL_GetAudioDeviceFormat()` 查询设备本地格式，避免不必要重采样
- 预加载音频数据到内存

---

## 十六、代码审查清单

- [ ] 所有 SDL3 资源是否通过 `std::unique_ptr` + 自定义删除器管理
- [ ] 是否**没有**手工调用的 `SDL_Destroy*` 函数
- [ ] 指针返回函数是否正确检查 `nullptr`
- [ ] `bool` 返回函数是否正确检查返回值
- [ ] `SDL_main.h` 是否只在**一个** `.cpp` 文件中包含
- [ ] Main Callbacks 是否设置了 `#define SDL_MAIN_USE_CALLBACKS`
- [ ] 事件枚举是否全部使用 SDL3 命名（`SDL_EVENT_*`）
- [ ] 渲染函数是否使用 SDL3 命名（`SDL_RenderTexture` 而非 `SDL_RenderCopy`）
- [ ] 窗口创建签名是否适配 SDL3（无 `x, y` 参数）
- [ ] 音频是否使用 `SDL_AudioStream`（非 SDL2 回调方式）
- [ ] 渲染操作是否仅在主线程执行
- [ ] 错误检查是否使用返回值（非仅依赖 `SDL_GetError()`）
- [ ] HiDPI 是否使用 `SDL_GetWindowSizeInPixels()`
- [ ] CMake 是否正确链接 `SDL3::SDL3`
- [ ] 跨平台是否使用 `SDL_PLATFORM_*` 宏
- [ ] 拷贝/移动构造是否正确实现（Rule of Five）
- [ ] 异常/错误处理策略在项目中是否保持一致

---

## 参考资料

- [SDL3 Wiki 首页](https://wiki.libsdl.org/SDL3/FrontPage)
- [SDL3 API 按类别浏览](https://wiki.libsdl.org/SDL3/APIByCategory)
- [SDL3 迁移指南](https://wiki.libsdl.org/SDL3/README-migration)
- [SDL3 新特性](https://wiki.libsdl.org/SDL3/NewFeatures)
- [SDL3 GPU API](https://wiki.libsdl.org/SDL3/CategoryGPU)
- [SDL3 CMake 集成](https://wiki.libsdl.org/SDL3/README-cmake)
- [SDL3 Main 函数与回调](https://wiki.libsdl.org/SDL3/README-main-functions)
- [SDL3 GitHub 仓库](https://github.com/libsdl-org/SDL)
