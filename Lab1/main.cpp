#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

void errorHandler(int errorCode, const char * description) {
  std::cerr << "Error: " << description << std::endl;
  glfwTerminate();
  exit(1);
}

GLFWwindow * createWindow(int width, int height, const char * title = "", GLFWmonitor *monitor = nullptr, GLFWwindow * share = nullptr) {


  GLFWwindow * win = glfwCreateWindow(width, height, title, monitor, share);
  if(!win) {
    glfwTerminate();
    exit(1);
  }

  return win;
}

void keyCallback(GLFWwindow * win, int key, int scancode, int action, int mode) {
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  }
}

int main() {

  if(!glfwInit())
    exit(1);
  glfwSetErrorCallback(errorHandler);

  GLFWwindow * win = createWindow(800, 600, "BAZA");

  glfwMakeContextCurrent(win);
  glfwSetKeyCallback(win, keyCallback);

  while(!glfwWindowShouldClose(win)) {
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwDestroyWindow(win);

  glfwTerminate();
  return 0;
}
