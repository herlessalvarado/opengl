cmake_minimum_required(VERSION 3.26)
project(opengl)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_executable(opengl main.cpp)

target_link_libraries(opengl PRIVATE glfw glad)