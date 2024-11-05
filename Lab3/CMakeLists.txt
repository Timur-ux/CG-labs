cmake_minimum_required(VERSION 3.10)

add_executable(Lab3_main main.cpp)

target_link_libraries(Lab3_main
  SOIL
  GLX
  glfw
  GLEW
  OpenGL
  ${SRC_LIB})

target_include_directories(Lab3_main PUBLIC "include")
target_compile_options(Lab3_main PUBLIC -g)


