cmake_minimum_required(VERSION 3.10)

project(snake VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(snake main.cpp)

if(MSVC)
    target_compile_options(snake PRIVATE /utf-8)
endif()

set(SDL3_DIR "D:/Library/SDL3-3.4.10/cmake")
set(SDL3_ttf_DIR "D:/Library/SDL3_ttf/cmake")
set(GLM_INCLUDE_DIRS "D:/Library/glm/include") 
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_include_directories(snake PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(snake PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# 复制 DLL 到输出目录
add_custom_command(TARGET snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:snake>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
        "$<TARGET_FILE_DIR:snake>"
)



