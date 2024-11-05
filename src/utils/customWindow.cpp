#include "utils/customWindow.hpp"

#include <iostream>

void errorHandler(int errorCode, const char *description) {
  std::cerr << "ERROR: " << description << std::endl;
  glfwTerminate();
  exit(1);
}

void setDefaultCtxParams() {
  glfwWindowHint(GLFW_RESIZABLE, 1);
  glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
  glfwWindowHint(GLFW_DEPTH_BITS, 1);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwSetErrorCallback(errorHandler);
}
