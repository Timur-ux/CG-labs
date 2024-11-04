#include "utils/OpenglInitializer.hpp"

#include <iostream>

OpenglInitializer::OpenglInitializer() {

  if (!glfwInit())
    std::cerr << "Error: GLFW initialization failed" << std::endl;
  glGetError();

  setDefaultCtxParams();
  window_ = glfwCreateWindow(1200, 800, "Lab 2", NULL, NULL);
  glfwMakeContextCurrent(window_);


  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

}
