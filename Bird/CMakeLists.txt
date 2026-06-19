cmake_minimum_required(VERSION 3.20)
project(Bird LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========== 指定 SDL3 各库 CMake 配置目录 ==========
set(SDL3_DIR        "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_image_DIR  "D:/Library/SDL3_image/cmake")
set(SDL3_mixer_DIR  "D:/Library/SDL3_mixer/cmake")

# ========== 通过 find_package 引入各库 ==========
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

# ========== 源文件 ==========
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Bird.cpp
    src/Pipe.cpp
    src/Ground.cpp
    src/Background.cpp
    src/Score.cpp
    src/Button.cpp
    src/UI.cpp
    src/Audio.cpp
)

set(HEADERS
    src/Game.h
    src/Bird.h
    src/Pipe.h
    src/Ground.h
    src/Background.h
    src/Score.h
    src/Button.h
    src/UI.h
    src/Audio.h
    src/Config.h
)

# ========== 可执行文件 ==========
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)

# ========== 自动拷贝 DLL 到输出目录 (Windows) ==========
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying SDL3.dll"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_image::SDL3_image>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying SDL3_image.dll"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying SDL3_mixer.dll"
)

# ========== 拷贝资源文件到输出目录 ==========
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets to output directory"
)