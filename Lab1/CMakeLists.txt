cmake_minimum_required(VERSION 3.10)

project(
  Lab1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)

find_package(glfw3 3.3.6 REQUIRED)

target_link_directories(main PUBLIC glfw)
target_link_libraries(main PUBLIC glfw)

