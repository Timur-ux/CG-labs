#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "customWindow.hpp"
#include <stdexcept>

class ICanvas {
public:
  virtual void draw(GLFWwindow *) = 0;
};

class WindowHandler {
  GLFWwindow *win_;

public:
  WindowHandler(WindowHandler &other) = delete;
  WindowHandler(GLFWwindow* win) : win_(win) {}

  void setKeyCallback(void (*callback)(GLFWwindow *, int, int, int, int)) {
    glfwSetKeyCallback(win_, callback);
  }

  void toForeground() {
    glfwMakeContextCurrent(win_); 
  }

  void startLoop(ICanvas &canvas) {
    while (!glfwWindowShouldClose(win_)) {
      canvas.draw(win_);
      glfwSwapBuffers(win_);
      glfwPollEvents();
    }
  }

  virtual ~WindowHandler() {}
};

GLFWwindow * createWindow(int width, int height, const char *title = "",
                GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr) { 
    GLFWwindow * win = glfwCreateWindow(width, height, title, monitor, share);
    if (!win) {
      throw std::runtime_error("Can't create window");
    }

    return win;
}


class Canvas : public ICanvas {
public:
  void draw(GLFWwindow *win) override {
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};


void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  }
}

int main() {

  if (!glfwInit())
    exit(1);

  setDefaultCtxParams();
  
  GLFWwindow * win = createWindow(800, 600, "Baza");
  WindowHandler winHandler(win);
  winHandler.toForeground();
  winHandler.setKeyCallback(keyCallback);

  glewInit();
  glGetError();


  Canvas canvas;
  winHandler.startLoop(canvas);

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}

