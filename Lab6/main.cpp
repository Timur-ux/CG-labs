#include "CameraMVP.hpp"
#include "EventHandlers/LookupHandler.hpp"
#include "EventHandlers/moveHandler.hpp"
#include "GlobalEvents.hpp"
#include "Light/BlinPhongLight.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "collaiders/intersections.hpp"
#include "events.hpp"
#include "fabrics/Box.hpp"
#include "glCheckError.hpp"
#include "utils/OpenglInitializer.hpp"
#include "utils/printUniforms.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#define GLEW_STATIC
#include "Object.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void keyCallback(GLFWwindow *, int, int, int, int);
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;

static OpenglInitializer initializer;

int main() {
  GLFWwindow *win = initializer.getWindow();
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

  // Объекты
  // Rectangle cube(glm::vec3(.5), glm::vec3(0, 2, 0), blinPhongProgram,
  //                containerTex);
  // collaider::AxisAlignedBB cubeAABB(.5f, .5f, .5f, &cube);
  // RigidBody rigidBody(&cube, 5);
  // engine::ObjectBase cubeBase;
  // cubeBase.setComponent(engine::ComponentType::transform, &cube);
  // cubeBase.setComponent(engine::ComponentType::collaider, &cubeAABB);
  // cubeBase.setComponent(engine::ComponentType::rigidBody, &rigidBody);
  // cubeBase.setComponent(engine::ComponentType::mesh, &cube);
  //
  // Rectangle floor(glm::vec3(50, 0.1, 50), glm::vec3(0, 0, 0), blinPhongProgram,
  //                 floorTex);
  // collaider::AxisAlignedBB floorAABB(50, 1.8, 50, &floor);
  // engine::ObjectBase floorBase;
  // floorBase.setComponent(engine::ComponentType::transform, &floor);
  // floorBase.setComponent(engine::ComponentType::collaider, &floorAABB);
  // floorBase.setComponent(engine::ComponentType::mesh, &floor);

  fabric::BoxFabric boxFabric("./textures/container.jpg", glm::vec3(0,2,0), glm::vec3(.5), glm::vec3 (0, 0, 1), 5, &blinPhongProgram);

  auto cube = boxFabric.create();
  boxFabric.mass = 0;
  boxFabric.pos = {0, 0, 0};
  boxFabric.size = {50, 0.1, 50};
  boxFabric.setTexture("./textures/stoneFloor.png");
  auto floor = boxFabric.create();

  // Камера
  CameraMVP cameraData(blinPhongProgram, glm::vec3(0, 2, 3), glm::vec3(0, 0, 0),
                       glm::vec3(0, 0, 0));
  // cameraData.follow(&cube, {0, 0, 5});

  // Источники света
  int LIGHTS_CNT = 1;
  glm::vec3 lightPos(10.0f, 15.0f, -1.0f);
  BlinPhongLight light1(lightPos, glm::vec3(0), cameraData, blinPhongProgram,
                        DepthFramebuffer(1200, 1200, 1));

  BlinPhongLight light2(lightPos * glm::vec3(-1, 1, 1), glm::vec3(0),
                        cameraData, blinPhongProgram,
                        DepthFramebuffer(1200, 1200, 1));

  // Сцены
  Scene scene(blinPhongProgram, cameraData, {&light1, &light2},
      {(Mesh*)cube->getComponent(engine::ComponentType::mesh), (Mesh*)floor->getComponent(engine::ComponentType::mesh)});

  // Event Handler'ы
  MoveEventHandler moveHandler1(cameraData);
  keyPressEvent += moveHandler1;
  LookupEventHandler lookupHandler(cameraData, win);
  // lookupHandler.lockY();
  mouseMoveEvent += lookupHandler;
  // lookupHandler.setPitchFov(30);
  LookupEventHandler lookupHandler2(cameraData, win);
  mouseMoveEvent += lookupHandler2;

  // Настройки шейдерной программы
  if (!blinPhongProgram.setUniformInt("main_texture0", 0))
    std::cerr << "Cant set main texture uniform!" << std::endl;
  if (!blinPhongProgram.setUniformInt("lightsCnt", LIGHTS_CNT))
    std::cerr << "Cant set quantity of light sources uniforms!" << std::endl;

  // Цикл рендера
  double time = glfwGetTime(), prevTime = time;
  float accumulator = 0;
  float dt = 0.02;
  while (!glfwWindowShouldClose(win)) {
    // Обновляем время
    prevTime = time;
    time = glfwGetTime();
    accumulator += time - prevTime;
    glCheckError();

    while(accumulator > 0.2) {
      accumulator -= dt;
      // рендерим сцену
      glClearColor(0, 0.2, 0.2, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      scene.update(time, dt);
      std::optional<collaider::CollisionData> collisionData =
          collaider::getIntersectionData(cube.get(), floor.get());
      if (collisionData.has_value()) {
        collaider::resolveCollision(collisionData.value());
      }
      GlobalEvents::updateEvent.invoke(time, dt);

      // Свапаем буферы glfwGetWindowSize(win, &width, &height);
      glViewport(0, 0, width, height);
      glfwSwapBuffers(win);

      // Обрабатываем ввод
      cameraData.updateState();
      glfwPollEvents();
      moveHandler1.move();
    }
  }
  glCheckError();

  // Освобождаем ресурсы
  glfwDestroyWindow(win);
  glfwTerminate();
  glCheckError();

  return 0;
}

void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  keyPressEvent.invoke(key, action, mods);
}

void mouseMoveCallback(GLFWwindow *win, double x, double y) {
  mouseMoveEvent.invoke(x, y);
}
