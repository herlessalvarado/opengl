cmake_minimum_required(VERSION 3.26)
project(opengl)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

set(GLM_ROOT glm)
add_subdirectory(${GLM_ROOT})

add_executable(opengl main.cpp
        stb_image.cpp)

target_link_libraries(opengl PRIVATE glfw glad glm assimp::assimp)