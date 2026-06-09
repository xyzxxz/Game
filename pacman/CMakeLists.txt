cmake_minimum_required(VERSION 3.20)

project(pacman)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/input.cpp
    src/resource.cpp
    src/model/map.cpp
    src/view/menu_renderer.cpp
    src/view/level_select_renderer.cpp
    src/view/stage_renderer.cpp
    src/view/hud_renderer.cpp
    src/logic/collision.cpp
    src/logic/ghost_ai.cpp
    src/logic/stage_controller.cpp
)

add_executable(pacman ${SOURCES})

if(MSVC)
    target_compile_options(pacman PRIVATE /utf-8)
endif()

set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_image_DIR "D:/Library/SDL3_image/cmake")

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

target_link_libraries(pacman PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

target_include_directories(pacman PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 复制 DLL 到输出目录
add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:pacman>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_image::SDL3_image>"
        "$<TARGET_FILE_DIR:pacman>"
)
