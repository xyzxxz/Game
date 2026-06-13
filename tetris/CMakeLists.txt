cmake_minimum_required(VERSION 3.10)

project(tetris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- 源文件 ----------
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/tetromino.cpp
    src/renderer.cpp
)

add_executable(tetris ${SOURCES})

# ---------- SDL3 依赖 ----------
set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_ttf_DIR "D:/Library/SDL3_ttf/cmake")
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(tetris PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# 复制 DLL 和字体到输出目录
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:tetris>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
        "$<TARGET_FILE_DIR:tetris>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/font.ttf"
        "$<TARGET_FILE_DIR:tetris>"
)