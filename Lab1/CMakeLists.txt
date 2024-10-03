cmake_minimum_required(VERSION 3.10)

add_executable(main main.cpp)

target_link_libraries(main
  glfw
  GLEW
  OpenGL
  ${SRC_LIB})

target_compile_options(main PUBLIC -g)
