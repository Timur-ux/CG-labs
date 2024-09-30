#include <cstdlib>
#include "customWindow.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Object.hpp"

int main() {

  glfwInit();
  glGetError(); // Вызов glfwInit может привести к ошибке, игнорируем её

  setDefaultCtxParams();
  Scene scene;

  GLFWwindow * win = glfwCreateWindow(800, 600, "Lab 1", NULL, NULL);
  ptr<Camera> camera = std::make_shared<Camera>(win);

  scene.addCamera(camera);

  while(!glfwWindowShouldClose(win)) {
    scene.update();
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  
  
  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}

