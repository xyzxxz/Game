cmake_minimum_required(VERSION 3.20)

project(Minesweeper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Minesweeper
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/menustate.cpp
    src/playingstate.cpp
    src/gameoverstate.cpp
)

set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_ttf_DIR "D:/Library/SDL3_ttf/cmake")

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(Minesweeper PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# 复制 DLL 到输出目录
add_custom_command(TARGET Minesweeper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:Minesweeper>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
        "$<TARGET_FILE_DIR:Minesweeper>"    
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/font.ttf"
        "$<TARGET_FILE_DIR:Minesweeper>/font.ttf"
)
