cmake_minimum_required(VERSION 3.20)
project(star-battle VERSION 1.0.0 LANGUAGES C CXX)

# C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 生成 compile_commands.json 供 clangd 使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 指定 SDL3 预编译库路径（Windows 本地）
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3")
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3_image")
list(APPEND CMAKE_PREFIX_PATH "D:/Library/SDL3_mixer")

# 查找 SDL3
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

# 收集源文件
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
)

# 创建可执行文件
add_executable(star-battle WIN32
    ${SOURCES}
)

# 链接 SDL3
target_link_libraries(star-battle PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)

# 包含目录
target_include_directories(star-battle PRIVATE src)

# 拷贝资源到构建目录
add_custom_command(TARGET star-battle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/img $<TARGET_FILE_DIR:star-battle>/img
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/sound $<TARGET_FILE_DIR:star-battle>/sound
)

# 拷贝 SDL3 DLL 到输出目录
add_custom_command(TARGET star-battle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:star-battle> $<TARGET_FILE_DIR:star-battle>
    COMMAND_EXPAND_LISTS
)