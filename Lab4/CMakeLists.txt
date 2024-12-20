cmake_minimum_required(VERSION 3.10)

add_executable(Lab4_main main.cpp)

target_link_libraries(Lab4_main
  SOIL
  GLX
  glfw
  GLEW
  OpenGL
  ${SRC_LIB})

target_include_directories(Lab4_main PUBLIC "include")
target_compile_options(Lab4_main PUBLIC -g)


