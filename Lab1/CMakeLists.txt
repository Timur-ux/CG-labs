cmake_minimum_required(VERSION 3.10)

add_executable(Lab1_main main.cpp)

target_link_libraries(Lab1_main
  glfw
  GLEW
  OpenGL
  ${SRC_OLD_LIB})

target_include_directories(Lab1_main PUBLIC "include_old")
target_compile_options(Lab1_main PUBLIC -g)
