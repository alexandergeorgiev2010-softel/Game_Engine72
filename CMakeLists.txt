cmake_minimum_required(VERSION 3.20)

project(Game_Engine72)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.5 REQUIRED PATHS /usr/local/Cellar/glfw/3.5.1/lib/cmake)

add_executable(Game_Engine72
    src/Engine/Application.cpp
    src/Engine/main.cpp
    src/Engine/Window/Window.cpp
    third_party/glad/src/gl.c
)

target_include_directories(Game_Engine72 PRIVATE
    src
    third_party/glad/include
)

target_link_libraries(Game_Engine72 PRIVATE
    glfw
)