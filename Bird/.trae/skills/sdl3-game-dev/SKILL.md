---
name: "sdl3-game-dev"
description: "SDL3 game development with C++ and CMake. Covers 2D Render API, GPU API, shaders, audio, input, project setup, and best practices. Uses pre-built SDL3 libraries (D:\\Library\\SDL3-3.4.10) with direct linking. Invoke when user wants to create a C++ game, game engine, graphics demo, or any SDL3-based project."
---

# SDL3 游戏开发智能体

本 skill 仅用于 **C++ + CMake** 项目，提供使用 SDL3 (Simple DirectMedia Layer 3) 进行游戏开发的完整指导，覆盖从项目搭建到渲染、输入、音频、资源管理等全流程。SDL3 是 SDL 的最新大版本（2025年发布），对 API 做了全面重构：简化初始化流程、统一设备枚举、引入全新 GPU API、支持 HDR 和高 DPI、提供回调式主循环等现代化特性。

## 库路径约定

所有 SDL3 预编译库均位于 `D:\Library\` 目录下，**不在项目中下载源码，直接链接预编译库**：

| 库          | 路径                       |
| ---------- | ------------------------ |
| SDL3       | `D:\Library\SDL3-3.4.10` |
| SDL3_image | `D:\Library\SDL3_image`  |
| SDL3_ttf   | `D:\Library\SDL3_ttf`    |
| SDL3_mixer | `D:\Library\SDL3_mixer`  |
| SDL3_net   | `D:\Library\SDL3_net`    |

每个库目录下标准结构：`include/`（头文件）、`lib/`（`.lib` 导入库）、`bin/`（`.dll` 动态库）、`cmake/`（CMake 配置文件，含 `SDL3Config.cmake` 等，供 `find_package` 使用）。

## 核心理念

- **纯 C++/CMake**：本项目 skill 仅针对 C++ 语言和 CMake 构建系统。
- **预编译库直链**：所有 SDL3 相关库使用预编译版本，通过 CMake 直接链接，不编译源码。
- **跨平台优先**：SDL3 抽象了 Windows(D3D12)、Linux(Vulkan)、macOS/iOS(Metal) 的底层差异，一次编写到处编译。
- **两条渲染路径**：(1) Render API — 简单 2D 硬件加速渲染；(2) GPU API — 类 Vulkan/Metal/D3D12 的低级 GPU 编程接口。
- **回调式主循环**：SDL3 推荐使用 `SDL_MAIN_USE_CALLBACKS` 宏，由 SDL 自动生成 `main` 函数并调用四个回调：`SDL_AppInit`、`SDL_AppEvent`、`SDL_AppIterate`、`SDL_AppQuit`。
- **zlib 许可证**：允许商业使用且无需开源。

---

## 1. 项目搭建

### 1.1 CMake 目录变量

在 CMakeLists.txt 中通过 `<库名>_DIR` 变量指向各预编译库的 `cmake/` 子目录，`find_package` 会从这些路径查找库的 CMake 配置文件（`SDL3Config.cmake` 等）：

```cmake
# 指定各库的 cmake 配置文件路径（指向 cmake/ 子目录）
set(SDL3_DIR        "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_image_DIR  "D:/Library/SDL3_image/cmake")
set(SDL3_ttf_DIR    "D:/Library/SDL3_ttf/cmake")
set(SDL3_mixer_DIR  "D:/Library/SDL3_mixer/cmake")
set(SDL3_net_DIR    "D:/Library/SDL3_net/cmake")
```

### 1.2 CMakeLists.txt 模板（仅 SDL3）

```cmake
cmake_minimum_required(VERSION 3.20)
project(my_game LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# ========== 指定 SDL3 CMake 配置目录 ==========
set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")

# ========== 通过 find_package 引入 SDL3 ==========
find_package(SDL3 REQUIRED CONFIG)

# ========== 主程序 ==========
add_executable(my_game
    src/main.cpp
)

target_link_libraries(my_game PRIVATE SDL3::SDL3)

# 自动拷贝 DLL 到输出目录 (Windows)
add_custom_command(TARGET my_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:my_game>"
    COMMENT "Copying SDL3.dll to output directory"
)
```

> `SDL3_DIR` 必须指向 `cmake/` 子目录（而不是库根目录），该目录下包含 `SDL3Config.cmake`。`find_package(SDL3 REQUIRED CONFIG)` 会自动解析头文件路径、导入库路径和 DLL 位置，无需手动设置 `IMPORTED_LOCATION` 等属性。`SDL3::SDL3` 是由 `find_package` 自动创建的 CMake 目标。

### 1.3 CMakeLists.txt 模板（含 SDL3_image + SDL3_ttf）

```cmake
cmake_minimum_required(VERSION 3.20)
project(my_game LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# ========== 指定各库 CMake 配置目录 ==========
set(SDL3_DIR        "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_image_DIR  "D:/Library/SDL3_image/cmake")
set(SDL3_ttf_DIR    "D:/Library/SDL3_ttf/cmake")

# ========== 通过 find_package 引入各库 ==========
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

# ========== 主程序 ==========
add_executable(my_game
    src/main.cpp
)

target_link_libraries(my_game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# 自动拷贝 DLL 到输出目录
foreach(DLL_TARGET SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
    add_custom_command(TARGET my_game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${DLL_TARGET}>"
            "$<TARGET_FILE_DIR:my_game>"
        COMMENT "Copying DLL to output directory"
    )
endforeach()
```

> **注意**：`CMAKE_CXX_STANDARD` 必须设为 20（C++20），SDL3 头文件依赖 C++20 特性。

### 1.4 最小 Hello World

```cpp
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    SDL_Log("Hello, SDL3!");
    return 0;
}
```

> **注意**：`#include <SDL3/SDL_main.h>` 会将 `main` 宏替换为 `SDL_main`，因此 `main` 必须写成带参数完整形式 `int main(int argc, char* argv[])`。

---

## 2. 回调式主循环（推荐架构）

SDL3 引入回调式应用架构，使用 `SDL_MAIN_USE_CALLBACKS` 宏替代传统 `main` 函数。四个回调函数分工明确：

```cpp
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// 应用全局状态
struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
};

// 1. 初始化 — 创建窗口、渲染器、加载资源
SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    auto* state = new AppState{};
    *appstate = state;

    SDL_SetAppMetadata("My Game", "1.0", "com.example.mygame");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = SDL_CreateWindow("My Game", 800, 600, 0);
    if (!state->window) return SDL_APP_FAILURE;

    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer) return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

// 2. 事件处理 — 每帧处理所有待处理事件
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    auto* state = static_cast<AppState*>(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        state->quit = true;
        return SDL_APP_SUCCESS;  // 返回 SUCCESS 会结束主循环
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                state->quit = true;
                return SDL_APP_SUCCESS;
            case SDLK_SPACE:
                SDL_Log("Space pressed!");
                break;
        }
    }

    return SDL_APP_CONTINUE;
}

// 3. 帧更新与渲染 — 每帧调用（不保证固定频率）
SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* state = static_cast<AppState*>(appstate);

    // 清除屏幕
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // 绘制...
    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    SDL_FRect rect = {100, 100, 200, 200};
    SDL_RenderFillRect(state->renderer, &rect);

    // 呈现
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

// 4. 清理 — 程序退出时调用
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    auto* state = static_cast<AppState*>(appstate);
    if (state->renderer) SDL_DestroyRenderer(state->renderer);
    if (state->window) SDL_DestroyWindow(state->window);
    delete state;
}
```

### 回调返回值约定

| 返回值                | 含义   |
| ------------------ | ---- |
| `SDL_APP_CONTINUE` | 继续运行 |
| `SDL_APP_SUCCESS`  | 正常退出 |
| `SDL_APP_FAILURE`  | 异常退出 |

- `SDL_AppInit` 返回 `SDL_APP_FAILURE` 时，`SDL_AppQuit` 不会被调用，需要自行清理。
- `SDL_AppEvent` 返回 `SDL_APP_SUCCESS` 或 `SDL_APP_FAILURE` 时，不再调用后续 `SDL_AppIterate`，直接转到 `SDL_AppQuit`。

---

## 3. Render API（2D 硬件加速渲染）

适合 2D 游戏、UI、简单图形应用。核心渲染流程：

```
创建窗口 → 创建渲染器 → 创建纹理 → 渲染循环(清屏→绘制→呈现)
```

### 3.1 创建渲染器

```cpp
// NULL 表示自动选择最佳后端 (Direct3D/OpenGL/Vulkan/Metal)
SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

// 也可指定后端名称
// SDL_CreateRenderer(window, "vulkan");
// SDL_CreateRenderer(window, "direct3d12");
// SDL_CreateRenderer(window, "opengl");

// 设置逻辑分辨率（自动缩放适配窗口大小）
SDL_SetRenderLogicalPresentation(renderer, 1920, 1080,
    SDL_LOGICAL_PRESENTATION_LETTERBOX);
```

### 3.2 纹理创建与渲染

```cpp
// 从图片文件加载纹理
SDL_Texture* texture = IMG_LoadTexture(renderer, "player.png");

// 从 Surface 创建纹理
SDL_Surface* surface = IMG_Load("player.png");
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
SDL_DestroySurface(surface);

// 渲染纹理（完整或裁剪）
SDL_FRect dst = {x, y, w, h};
SDL_RenderTexture(renderer, texture, NULL, &dst);

// 带旋转、翻转的渲染
SDL_RenderTextureRotated(renderer, texture, NULL, &dst,
    angle_degrees, &center, SDL_FLIP_HORIZONTAL);

// 颜色调制（改变纹理色调）
SDL_SetTextureColorMod(texture, 255, 128, 64);   // RGB
SDL_SetTextureAlphaMod(texture, 200);              // 透明度 0-255
```

### 3.3 几何绘制（无纹理）

```cpp
// 矩形
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_FRect rect = {50, 50, 100, 100};
SDL_RenderFillRect(renderer, &rect);    // 填充
SDL_RenderRect(renderer, &rect);        // 边框

// 线段
SDL_RenderLine(renderer, x1, y1, x2, y2);

// 点
SDL_RenderPoint(renderer, x, y);

// 自定义三角形（需开启几何渲染）
// SDL_SetRenderDrawColor(renderer, ...);
// SDL_RenderGeometry(renderer, NULL, vertices, num_vertices, indices, num_indices);
```

### 3.4 视口与裁剪

```cpp
// 设置渲染视口
SDL_Rect viewport = {0, 0, 400, 300};
SDL_SetRenderViewport(renderer, &viewport);

// 设置裁剪区域
SDL_Rect clip = {100, 100, 200, 200};
SDL_SetRenderClipRect(renderer, &clip);
```

### 3.5 批量渲染技巧

```cpp
// 批量绘制大量精灵时，先按纹理排序减少纹理切换
// 使用 SDL_RenderTexture 循环即可
for (auto& sprite : sprites_sorted_by_texture) {
    SDL_RenderTexture(renderer, sprite.texture, &sprite.src, &sprite.dst);
}
```

---

## 4. GPU API（低级 GPU 编程）

GPU API 是 SDL3 最大的新特性，提供跨平台的低级 GPU 访问能力，支持 3D 渲染和 GPU 计算。

### 4.1 GPU 渲染流程概览

```
创建窗口 → 创建 GPU 设备 → 声明窗口 → 准备资源 → 帧循环
    │
    ├── 静态资源（创建一次复用）:
    │   ├── 着色器 (SDL_CreateGPUShader)
    │   ├── 缓冲区/顶点缓冲 (SDL_CreateGPUBuffer)
    │   ├── 纹理 (SDL_CreateGPUTexture)
    │   ├── 采样器 (SDL_CreateGPUSampler)
    │   └── 渲染管线 (SDL_CreateGPUGraphicsPipeline)
    │
    └── 每帧操作:
        ├── 获取命令缓冲区 (SDL_AcquireGPUCommandBuffer)
        ├── 获取交换链纹理 (SDL_WaitAndAcquireGPUSwapchainTexture)
        ├── 开始渲染通道 (SDL_BeginGPURenderPass)
        ├── 绑定管线/缓冲区/纹理/采样器
        ├── 绘制调用 (SDL_DrawGPUPrimitives / SDL_DrawGPUIndexedPrimitives)
        ├── 结束渲染通道 (SDL_EndGPURenderPass)
        └── 提交命令缓冲区 (SDL_SubmitGPUCommandBuffer)
```

### 4.2 创建 GPU 设备

```cpp
// 自动选择最佳后端
SDL_GPUDevice* device = SDL_CreateGPUDevice(
    SDL_GPU_SHADERFORMAT_SPIRV |   // Vulkan
    SDL_GPU_SHADERFORMAT_DXIL |    // D3D12
    SDL_GPU_SHADERFORMAT_MSL,      // Metal
    true,   // 开启 debug 模式
    NULL    // 驱动名称(NULL=自动选择)
);

// 将设备关联到窗口
SDL_ClaimWindowForGPUDevice(device, window);
```

### 4.3 数据上传：TransferBuffer

数据从 CPU 传到 GPU 需要 TransferBuffer：

```cpp
// 1. 创建 TransferBuffer（用于上传）
SDL_GPUTransferBufferCreateInfo tbInfo{};
tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
tbInfo.size = vertexDataSize + indexDataSize;
SDL_GPUTransferBuffer* transferBuf = SDL_CreateGPUTransferBuffer(device, &tbInfo);

// 2. 映射到 CPU 可写内存
void* mapped = SDL_MapGPUTransferBuffer(device, transferBuf, false);
memcpy(mapped, vertices, vertexDataSize);
memcpy((char*)mapped + vertexDataSize, indices, indexDataSize);
SDL_UnmapGPUTransferBuffer(device, transferBuf);

// 3. 通过 CopyPass 上传到 GPU Buffer
SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

SDL_GPUTransferBufferLocation srcLocation{};
srcLocation.transfer_buffer = transferBuf;
srcLocation.offset = 0;

SDL_GPUBufferRegion dstRegion{};
dstRegion.buffer = gpuVertexBuffer;
dstRegion.offset = 0;
dstRegion.size = vertexDataSize;

SDL_UploadToGPUBuffer(copyPass, &srcLocation, &dstRegion, false);
SDL_EndGPUCopyPass(copyPass);
SDL_SubmitGPUCommandBuffer(cmd);

// 4. 释放 TransferBuffer
SDL_ReleaseGPUTransferBuffer(device, transferBuf);
```

### 4.4 纹理上传

```cpp
// 创建 GPU 纹理
SDL_GPUTextureCreateInfo texInfo{};
texInfo.type = SDL_GPU_TEXTURETYPE_2D;
texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
texInfo.width = imageWidth;
texInfo.height = imageHeight;
texInfo.layer_count_or_depth = 1;
texInfo.num_levels = 1;
SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &texInfo);

// 通过 TransferBuffer + CopyPass 上传纹理数据
// ... (类似 Buffer 上传，使用 SDL_UploadToGPUTexture)
```

### 4.5 渲染管线创建

```cpp
// 顶点属性描述
SDL_GPUVertexAttribute vertexAttrs[2] = {};
vertexAttrs[0].location = 0;
vertexAttrs[0].buffer_slot = 0;
vertexAttrs[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;  // position
vertexAttrs[0].offset = 0;

vertexAttrs[1].location = 1;
vertexAttrs[1].buffer_slot = 0;
vertexAttrs[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;  // uv
vertexAttrs[1].offset = sizeof(float) * 2;

// 顶点输入状态
SDL_GPUVertexInputState vertexInput{};
vertexInput.num_vertex_buffers = 1;
vertexInput.vertex_buffer_descriptions = &(SDL_GPUVertexBufferDescription){
    .slot = 0,
    .pitch = sizeof(Vertex),  // struct Vertex { float x,y; float u,v; }
    .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
    .instance_step_rate = 0,
};
vertexInput.num_vertex_attributes = 2;
vertexInput.vertex_attributes = vertexAttrs;

// 光栅化状态
SDL_GPURasterizerState rasterizer{};
rasterizer.fill_mode = SDL_GPU_FILLMODE_FILL;
rasterizer.cull_mode = SDL_GPU_CULLMODE_NONE;
rasterizer.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

// 混合状态
SDL_GPUColorTargetBlendState blend{};
blend.enable_blend = true;
blend.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
blend.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
blend.color_blend_op = SDL_GPU_BLENDOP_ADD;
blend.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
blend.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;

// 渲染目标描述
SDL_GPUColorTargetDescription colorTarget{};
colorTarget.format = SDL_GetGPUSwapchainTextureFormat(device, window);
colorTarget.blend_state = blend;

SDL_GPUGraphicsPipelineTargetInfo targetInfo{};
targetInfo.num_color_targets = 1;
targetInfo.color_target_descriptions = &colorTarget;

// 创建管线
SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
pipelineInfo.vertex_shader = vertexShader;
pipelineInfo.fragment_shader = fragmentShader;
pipelineInfo.vertex_input_state = vertexInput;
pipelineInfo.rasterizer_state = rasterizer;
pipelineInfo.target_info = targetInfo;
pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
```

### 4.6 每帧渲染代码

```cpp
// 获取命令缓冲区
SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);

// 获取交换链纹理
SDL_GPUTexture* swapchainTexture;
Uint32 w, h;
if (!SDL_WaitAndAcquireGPUSwapchainTexture(device, window, &swapchainTexture, &w, &h)) {
    SDL_Log("Swapchain error: %s", SDL_GetError());
}

// 颜色目标
SDL_GPUColorTargetInfo colorTargetInfo{};
colorTargetInfo.texture = swapchainTexture;
colorTargetInfo.clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
colorTargetInfo.cycle = true;  // 避免帧间数据依赖

// 开始渲染通道
SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmd, &colorTargetInfo, 1, NULL);

// 绑定资源并绘制
SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
SDL_SetGPUViewport(renderPass, &(SDL_GPUViewport){0, 0, (float)w, (float)h, 0, 1});
SDL_BindGPUVertexBuffers(renderPass, 0, &(SDL_GPUBufferBinding){vertexBuffer, 0}, 1);
SDL_BindGPUIndexBuffer(renderPass, &(SDL_GPUBufferBinding){indexBuffer, 0}, SDL_GPU_INDEXELEMENTSIZE_16BIT);
SDL_PushGPUVertexUniformData(cmd, 0, &mvp, sizeof(glm::mat4));
SDL_PushGPUFragmentUniformData(cmd, 0, &color, sizeof(float) * 4);
SDL_DrawGPUIndexedPrimitives(renderPass, indexCount, 1, 0, 0, 0);

// 结束渲染通道
SDL_EndGPURenderPass(renderPass);

// 提交
SDL_SubmitGPUCommandBuffer(cmd);
```

### 4.7 GPU Buffer 的 Cycling 机制

GPU API 中的 Buffer/Texture/TransferBuffer 内部实现为环形缓冲区。几乎所有写入操作都有 `cycle` 参数：

- **`cycle = true`**：自动切换到下一个未占用的内部资源，避免等待 GPU 完成上一帧操作。**强烈推荐**每帧都设为 true，这样无需手动追踪资源依赖。
- **`cycle = false`**：复用同一内部资源，仅在确定数据已不再被 GPU 使用时使用。
- **警告**：cycling 后资源数据变为未定义状态，必须重新写入全部数据再使用。

---

## 5. 着色器规范

### 5.1 着色器格式选择

| 后端     | 着色器格式                                 | 平台                      |
| ------ | ------------------------------------- | ----------------------- |
| Vulkan | SPIR-V (`SDL_GPU_SHADERFORMAT_SPIRV`) | Windows, Linux, Android |
| D3D12  | DXIL (`SDL_GPU_SHADERFORMAT_DXIL`)    | Windows, Xbox           |
| Metal  | MSL (`SDL_GPU_SHADERFORMAT_MSL`)      | macOS, iOS              |

使用 [SDL_shadercross](https://github.com/libsdl-org/SDL_shadercross) 进行运行时或离线着色器交叉编译。

### 5.2 SPIR-V 资源绑定顺序（非常重要！）

SDL3 GPU API 对着色器资源布局有严格要求，不遵守将导致着色器无法正常工作：

**顶点着色器 (Vertex Shader):**

- Set 0：采样纹理 → 存储纹理 → 存储缓冲区
- Set 1：Uniform 缓冲区

**片元着色器 (Fragment Shader):**

- Set 2：采样纹理 → 存储纹理 → 存储缓冲区
- Set 3：Uniform 缓冲区

**计算着色器 (Compute Shader):**

- Set 4：采样纹理 → 存储纹理 → 存储缓冲区
- Set 5：Uniform 缓冲区

### 5.3 SPIR-V 着色器示例

```glsl
// vertex shader (vert.glsl → vert.spv)
#version 450

// Uniform 必须在 set=1
layout(set = 1, binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 fragUV;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPosition, 0.0, 1.0);
    fragUV = inUV;
}
```

```glsl
// fragment shader (frag.glsl → frag.spv)
#version 450

// 采样纹理必须在 set=2
layout(set = 2, binding = 0) uniform sampler2D texSampler;

// Uniform 必须在 set=3
layout(set = 3, binding = 0) uniform FragUniforms {
    vec4 color;
} fragUniforms;

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragUV) * fragUniforms.color;
}
```

### 5.4 编译 SPIR-V

```bash
glslangValidator -V vert.glsl -o vert.spv
glslangValidator -V frag.glsl -o frag.spv
```

### 5.5 Uniform 数据推送

SDL3 GPU API 提供 4 个 uniform slot（槽位）每阶段：

```cpp
// 推送顶点着色器 uniform（slot 0）
SDL_PushGPUVertexUniformData(cmd, 0, &mvpMatrix, sizeof(glm::mat4));

// 推送片元着色器 uniform（slot 0）
SDL_PushGPUFragmentUniformData(cmd, 0, &colorData, sizeof(float) * 4);

// 适用于少量数据（≤ 几个矩阵），大量数据应使用 Storage Buffer
```

---

## 6. 输入处理

### 6.1 键盘输入

```cpp
// 在 SDL_AppEvent 中处理
if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
        case SDLK_W:      moveForward = true;  break;
        case SDLK_S:      moveBack    = true;  break;
        case SDLK_A:      moveLeft    = true;  break;
        case SDLK_D:      moveRight   = true;  break;
        case SDLK_SPACE:  jump        = true;  break;
    }
}
if (event->type == SDL_EVENT_KEY_UP) {
    switch (event->key.key) {
        case SDLK_W:      moveForward = false; break;
        // ...
    }
}

// 持续按键检测（在 SDL_AppIterate 中使用）
const bool* keyboardState = SDL_GetKeyboardState(NULL);
if (keyboardState[SDL_SCANCODE_W]) { /* 持续移动 */ }
```

### 6.2 鼠标输入

```cpp
// 鼠标移动
if (event->type == SDL_EVENT_MOUSE_MOTION) {
    float dx = event->motion.xrel;
    float dy = event->motion.yrel;
}

// 鼠标按键
if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    if (event->button.button == SDL_BUTTON_LEFT) { /* ... */ }
}

// 鼠标滚轮
if (event->type == SDL_EVENT_MOUSE_WHEEL) {
    float scrollY = event->wheel.y;
}
```

### 6.3 游戏手柄

```cpp
// 检测手柄插入/拔出
if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
    SDL_Gamepad* gamepad = SDL_OpenGamepad(event->gdevice.which);
}

// 读取手柄状态（在 SDL_AppIterate 中）
float leftX  = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
bool  aButton = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_SOUTH);
```

### 6.4 触摸输入

```cpp
if (event->type == SDL_EVENT_FINGER_DOWN) {
    float x = event->tfinger.x;  // 0.0 ~ 1.0
    float y = event->tfinger.y;
}
```

---

## 7. 音频

```cpp
// 初始化音频子系统
SDL_Init(SDL_INIT_AUDIO);

// 打开音频设备
SDL_AudioSpec spec{};
spec.format = SDL_AUDIO_S16;
spec.channels = 2;
spec.freq = 44100;
SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

// 播放 WAV 文件
SDL_AudioSpec wavSpec;
Uint8* wavData;
Uint32 wavLen;
SDL_LoadWAV("sound.wav", &wavSpec, &wavData, &wavLen);
SDL_PutAudioStreamData(stream, wavData, wavLen);
SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(stream));
SDL_free(wavData);

// 或使用 SDL3_mixer（第三方扩展）简化音频管理
```

---

## 8. 扩展库

所有扩展库位于 `D:\Library\` 下，与 SDL3 同级。在 CMakeLists.txt 中通过 `find_package` 引入（需先以 `<库名>_DIR` 指定其 `cmake/` 目录）：

| 库          | 用途                      | 头文件                                 | CMake 目标                    |
| ---------- | ----------------------- | ----------------------------------- | --------------------------- |
| SDL3_image | 加载 PNG/JPG/WebP 等多种图片格式 | `#include <SDL3_image/SDL_image.h>` | `SDL3_image::SDL3_image`    |
| SDL3_ttf   | TrueType 字体渲染           | `#include <SDL3_ttf/SDL_ttf.h>`     | `SDL3_ttf::SDL3_ttf`        |
| SDL3_mixer | 音频混音（多声道、特效）            | `#include <SDL3_mixer/SDL_mixer.h>` | `SDL3_mixer::SDL3_mixer`    |
| SDL3_net   | 网络通信                    | `#include <SDL3_net/SDL_net.h>`     | `SDL3_net::SDL3_net`        |

每个扩展库的引入方式与 SDL3 完全相同（见 1.3 节），只需设置对应的 `<库名>_DIR` 并调用 `find_package` 即可。

---

## 9. 性能最佳实践

### 9.1 Render API 性能

- 减少 `SDL_RenderClear` 调用——如果每帧必定覆盖全屏，可以跳过 Clear。
- 按纹理分组绘制（Texture Atlas 优于多纹理切换）。
- 使用 `SDL_SetRenderLogicalPresentation` 配合固定逻辑分辨率。
- 大量精灵考虑使用 `SDL_RenderGeometry` 批量提交。

### 9.2 GPU API 性能

1. **减少 Render Pass 数量**：开始新的 Render Pass 开销较高，尽量合并。
2. **减少状态切换**：按管线/纹理分组绘制调用。
3. **提前上传数据**：在帧开始时完成所有 TransferBuffer 操作。
4. **不要频繁创建/销毁资源**：在 `SDL_AppInit` 中一次性创建，复用到程序结束。
5. **Uniform Buffer 只用于小数据（≤ 几个矩阵）**：大量数据使用 Storage Buffer。
6. **正确使用 Cycling**：每帧写操作设置 `cycle = true` 避免 GPU-CPU 同步等待。
7. **使用剔除技术**：视锥剔除、遮挡剔除减少像素写入。

### 9.3 通用性能

- 使用 RenderDoc 分析 GPU 帧性能。
- 避免在 `SDL_AppIterate` 中分配内存——预分配所有缓冲区。
- 使用固定时间步长（Fixed Timestep）解耦逻辑与渲染帧率：

```cpp
const double FIXED_DT = 1.0 / 60.0;
static double accumulator = 0.0;
Uint64 now = SDL_GetTicksNS();
double frameTime = (now - lastTime) / 1e9;
lastTime = now;
accumulator += frameTime;
while (accumulator >= FIXED_DT) {
    FixedUpdate(FIXED_DT);
    accumulator -= FIXED_DT;
}
Render(accumulator / FIXED_DT);  // 插值因子
```

---

## 10. 调试

### 10.1 日志

```cpp
SDL_Log("Info: %s", message);
SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s", SDL_GetError());
SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Warning...");
```

### 10.2 GPU 帧调试

- **Windows/Linux**：使用 [RenderDoc](https://renderdoc.org/) 捕获帧、查看 DrawCall、检查缓冲区。
- **macOS (Metal)**：Xcode → Debug → GPU Frame Capture（选择 Metal 捕捉）。

### 10.3 GPU Debug 模式

```cpp
// 创建 GPU 设备时开启 debug
SDL_GPUDevice* device = SDL_CreateGPUDevice(
    SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);  // true = debug mode
```

---

## 11. 常见陷阱与 FAQ

### 11.1 main 函数被宏替换

包含 `SDL_main.h` 时，`main` 被替换为 `SDL_main`。必须使用完整声明 `int main(int argc, char* argv[])`，**不能**写 `int main()`。

### 11.2 动态库未找到 (Windows)

- CMake 已通过 `add_custom_command(TARGET ... POST_BUILD)` 自动将 `.dll` 拷贝到输出目录（使用 `$<TARGET_FILE:SDL3::SDL3>` 表达式）。
- 手动运行 exe 时需确保 `.dll` 文件与可执行文件在同目录。
- 如遇 `find_package` 找不到库：检查 `<库名>_DIR` 是否正确指向 `cmake/` 子目录，且该目录下存在 `SDL3Config.cmake`。

### 11.3 着色器不工作

- 检查资源绑定 set 编号是否符合 SDL3 规范（SPIR-V: vert set 0/1, frag set 2/3, compute set 4/5）。
- 使用着色器反射工具自动提取资源布局而非手动填写。
- 检查 `SDL_GPUShaderCreateInfo` 中的 `num_samplers`、`num_storage_textures`、`num_storage_buffers` 是否匹配。

### 11.4 SDL_Init 失败

- 确保初始化对应子系统：`SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)`。
- 去掉不需要的子系统的 init 标志可以减少初始化开销。

### 11.5 窗口不显示

- SDL3 回调模式中，如果 `SDL_AppInit` 返回 `SDL_APP_FAILURE`，窗口不会被显示且 `SDL_AppQuit` 不会被调用。
- 确认 `SDL_AppIterate` 返回 `SDL_APP_CONTINUE`。

---

## 12. 项目模板推荐结构

```
my_game/
├── CMakeLists.txt          # 构建配置（含 find_package 库引入）
├── assets/                 # 游戏资源（图片、音频、字体）
│   ├── textures/
│   ├── audio/
│   ├── shaders/            # 着色器源码和编译后 .spv/.dxil/.msl
│   └── fonts/
├── src/
│   ├── main.cpp            # 回调入口
│   ├── Game.h / Game.cpp   # 游戏主逻辑
│   ├── Renderer.h          # 渲染封装
│   ├── Input.h             # 输入管理
│   ├── Audio.h             # 音频管理
│   └── Scene.h             # 场景管理
└── README.md
```

> **注意**：项目内不包含任何 SDL3 源码或第三方库。所有库通过 `D:\Library\` 下的预编译版本直接链接。

---

## 13. 设计决策指南

当面临以下选择时，按此指南决策：

| 场景                    | 推荐方案                     |
| --------------------- | ------------------------ |
| 纯 2D 像素/精灵游戏          | Render API               |
| 需要自定义着色器效果            | GPU API                  |
| 3D 游戏                 | GPU API                  |
| 需要 GPU 计算 (Compute)   | GPU API                  |
| 快速原型 / Game Jam       | Render API（回调模式）         |
| 需要与外部 D3D/Vulkan 代码交互 | GPU API（可获取原生句柄）         |
| 视频播放器                 | Render API + SDL3 纹理流式更新 |

---

## 参考文献

- [SDL 官方网站](https://www.libsdl.org/)
- [SDL3 GitHub 仓库](https://github.com/libsdl-org/SDL)
- [SDL3 GPU API 官方 Wiki](https://wiki.libsdl.org/SDL3/CategoryGPU)
- [SDL_shadercross 着色器交叉编译](https://github.com/libsdl-org/SDL_shadercross)
- [SDL GPU 示例代码](https://github.com/TheSpydog/SDL_gpu_examples)
- [RenderDoc 图形调试器](https://renderdoc.org/)
