cmake_minimum_required(VERSION 3.10)

add_executable(Lab6_main main.cpp)

target_link_libraries(Lab6_main
  SOIL
  GLX
  glfw
  GLEW
  OpenGL
  ${SRC_LIB})

target_include_directories(Lab6_main PUBLIC "include")
target_compile_options(Lab6_main PUBLIC -g)


