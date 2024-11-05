cmake_minimum_required(VERSION 3.10)

add_executable(Lab2_main main.cpp)

target_link_libraries(Lab2_main
  SOIL
  GLX
  glfw
  GLEW
  OpenGL
  ${SRC_LIB})

target_include_directories(Lab2_main PUBLIC "include")
target_compile_options(Lab2_main PUBLIC -g)

