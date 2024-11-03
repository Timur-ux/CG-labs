#include "CameraMVP.hpp"
#include "Light/Lambert.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "objects/Rectangle.hpp"
#include "utils/printUniforms.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLEW_STATIC
#include "customWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EventHandlers/moveHandler.hpp"
#include "EventHandlers/LookupHandler.hpp"
#include "MoveObjectFN.hpp"

void keyCallback(GLFWwindow *, int, int, int, int);
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;



int main() {
  if (!glfwInit())
    std::cerr << "Error: GLFW initialization failed" << std::endl;
  glGetError();

  setDefaultCtxParams();
  GLFWwindow *win = glfwCreateWindow(1200, 800, "Lab 2", NULL, NULL);
  glfwMakeContextCurrent(win);


  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

  // Настройки opengl
  int width, height;
  glfwGetWindowSize(win, &width, &height);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(255);

  // Callback'и
  glfwSetKeyCallback(win, keyCallback);
  glfwSetCursorPosCallback(win, mouseMoveCallback);

  // Шейдерная программа для основного рендера
  Program program("./shaders/Lab2Shader1(Lambert).glsl");
  eTB_GLSL__print_uniforms(program.get());
 
  // Текстуры
  Texture2D containerTex("./textures/container.jpg", 0);
  Texture2D sunTex("./textures/sun3.png", 0);

  // Объекты
  glm::vec3 lightPos(-1.0f, 30.0f, -1.0f);
  Rectangle sun(glm::vec3(1), lightPos, program, sunTex);
  Rectangle cube(glm::vec3(1), glm::vec3(0, 0, 0), program, containerTex, true);
  MoveObjectFN moveCubeFN(&cube);
  Rectangle xLine(glm::vec3(0.3), sun.position() + glm::vec3(2, 0, 0), program, containerTex);
  Rectangle yLine(glm::vec3(0.6), sun.position() + glm::vec3(0, 2, 0), program, containerTex);
  Rectangle yNegLine(glm::vec3(0.6), sun.position() - glm::vec3(0, 2, 0), program, containerTex);
  Rectangle zLine(glm::vec3(0.9), sun.position() + glm::vec3(0, 0, 2), program, containerTex);
  Rectangle floor(glm::vec3(50, 0.1, 50), glm::vec3(0, -1, -25), program, containerTex);
  glCheckError();

  // Камера
  CameraMVP cameraData(program, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  // Event Handler'ы
  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;
  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  // Источники света
  LambertLight light(lightPos, glm::vec3(0), cameraData, program, DepthFramebuffer(1200, 1200, 1));

  // Сцены
  Scene scene(program, cameraData, {&light}, { &cube, &xLine, &yLine,&yNegLine, &floor});

  // Настройки шейдерной программы
  program.setUniformInt("main_texture0", 0);

  // Цикл рендера
  double time = glfwGetTime(), prevTime = time;
  while (!glfwWindowShouldClose(win)) {
    // Обновляем время
    prevTime = time;
    time = glfwGetTime();
    float dt = time - prevTime;
    glCheckError();

    // рендерим сцену
    glClearColor(0, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.update(time, dt);
    moveCubeFN(time);

    // Свапаем буферы glfwGetWindowSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapBuffers(win);

    // Обрабатываем ввод
    cameraData.updateState();
    glfwPollEvents();
    moveHandler.move();
  }
  glCheckError();

  // Освобождаем ресурсы
  glfwDestroyWindow(win);
  glfwTerminate();
  glCheckError();

  return 0;
}

void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  keyPressEvent.invoke(key, action, mods);
}

void mouseMoveCallback(GLFWwindow * win, double x, double y) {
  mouseMoveEvent.invoke(x, y);
}
