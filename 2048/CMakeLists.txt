cmake_minimum_required(VERSION 3.28)

project(2048)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(2048 WIN32
    src/main.cpp
    src/game.cpp
    src/renderer.cpp
    src/app.cpp
)

if(MSVC)
    target_compile_options(2048 PRIVATE /utf-8)
endif()

set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_ttf_DIR "D:/Library/SDL3_ttf/cmake")

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(2048 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# 复制 DLL 到输出目录
add_custom_command(TARGET 2048 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:2048>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
        "$<TARGET_FILE_DIR:2048>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/font.ttf"
        "$<TARGET_FILE_DIR:2048>/font.ttf"
)
