cmake_minimum_required(VERSION 3.10)

project(
  CGLabs
  LANGUAGES CXX  
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3.6 REQUIRED)
find_package(GLEW 2.1.0 REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include_directories("./include/")
file(GLOB_RECURSE SRC LIST_DIRECTORIES FALSE ./src/*.cpp)

set(SRC_LIB src_lib)
add_library(${SRC_LIB} STATIC ${SRC})

add_subdirectory("./Lab1/")
