cmake_minimum_required(VERSION 3.10)

project(
  CGLabs
  LANGUAGES CXX  
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

find_package(glfw3 3.3.6 REQUIRED)
find_package(GLEW 2.1.0 REQUIRED)
find_package(glm CONFIG REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC LIST_DIRECTORIES FALSE src/*.cpp)
file(GLOB_RECURSE SRC_OLD LIST_DIRECTORIES FALSE src_old/*.cpp)

set(SRC_LIB src_lib)
set(SRC_OLD_LIB src_old_lib)

add_library(${SRC_LIB} STATIC ${SRC})
add_library(${SRC_OLD_LIB} STATIC ${SRC_OLD})

target_include_directories(${SRC_OLD_LIB} PUBLIC "include_old")
target_include_directories(${SRC_LIB} PUBLIC  "include")
target_link_libraries(${SRC_LIB} libSOIL.a GLX glfw OpenGL GLEW)
target_link_libraries(${SRC_OLD_LIB} glfw OpenGL GLEW glm::glm-header-only)
add_compile_definitions(DEBUG)

add_subdirectory("./Lab1/")
add_subdirectory("./Lab2/")
add_subdirectory("./Lab3/")
add_subdirectory("./Lab4/")
add_subdirectory("./Lab5/")
add_subdirectory("./Lab6/")
