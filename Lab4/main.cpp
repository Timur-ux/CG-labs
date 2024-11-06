#include "CameraMVP.hpp"
#include "Light/BlinPhongLight.hpp"
#include "Light/Lambert.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "objects/Pyramid.hpp"
#include "objects/Rectangle.hpp"
#include "objects/Sphere.hpp"
#include "utils/OpenglInitializer.hpp"
#include "utils/printUniforms.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLEW_STATIC
#include "utils/customWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EventHandlers/moveHandler.hpp"
#include "EventHandlers/LookupHandler.hpp"
#include "MoveObjectFN.hpp"

void keyCallback(GLFWwindow *, int, int, int, int);
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;


static OpenglInitializer initializer;

int main() {
  GLFWwindow * win = initializer.getWindow();
  glfwMakeContextCurrent(win);
  // Настройки opengl
  int width, height;
  glfwGetWindowSize(win, &width, &height);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  // Callback'и
  glfwSetKeyCallback(win, keyCallback);
  glfwSetCursorPosCallback(win, mouseMoveCallback);

  // Шейдерная программа для основного рендера
  Program blinPhongProgram("./shaders/BlinPhong_shadow.glsl");
  eTB_GLSL__print_uniforms(blinPhongProgram.get());
 
  // Текстуры
  Texture2D containerTex("./textures/container.jpg", 0);
  Texture2D floorTex("./textures/stoneFloor.png", 0);
  Texture2D * redColTex = Texture2D::createMonocolor(1200, 1200, glm::vec3(1, 0, 0));

  // Объекты
  Rectangle cube(glm::vec3(1), glm::vec3(0, 1, 0), blinPhongProgram, containerTex);
  Rectangle floor(glm::vec3(50, 0.1, 50), glm::vec3(0, -1, -25), blinPhongProgram, floorTex);
  Sphere bloodSphere(1, glm::vec3(7, 3, 7), blinPhongProgram, &containerTex);
  Pyramid pyramid(glm::vec3(-2, 3, -2), blinPhongProgram, redColTex);
  glCheckError();

  // Камера
  CameraMVP cameraData(blinPhongProgram, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  // Event Handler'ы
  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;
  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  // Источники света
  int LIGHTS_CNT = 2;
  glm::vec3 lightPos(10.0f, 15.0f, -1.0f);
  BlinPhongLight light1(lightPos, glm::vec3(0), cameraData, blinPhongProgram, DepthFramebuffer(1200, 1200, 1));

  BlinPhongLight light2(lightPos*glm::vec3(-1, 1, 1), glm::vec3(0), cameraData, blinPhongProgram, DepthFramebuffer(1200, 1200, 1));

  // Сцены
  Scene scene(blinPhongProgram, cameraData, {&light1, &light2}, { &cube, &floor, &bloodSphere, &pyramid});

  // Настройки шейдерной программы
  if(!blinPhongProgram.setUniformInt("main_texture0", 0))
    std::cerr << "Cant set main texture uniform!" << std::endl;
  if(! blinPhongProgram.setUniformInt("lightsCnt", LIGHTS_CNT)) 
    std::cerr << "Cant set quantity of light sources uniforms!" << std::endl;

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
  delete redColTex;
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

