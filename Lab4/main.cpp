#include "CameraMVP.hpp"
#include "Light/BlinPhongLight.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "meshes/Pyramid.hpp"
#include "meshes/Rectangle.hpp"
#include "meshes/Sphere.hpp"
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
#include "EventHandlers/LookupHandler.hpp"
#include "EventHandlers/moveHandler.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Для того, чтобы управлять включением/отключением света создадим подкласс
// Scene с возможностью отключения света Также созданим подкласс хендлера ввода
// с клавиатуры с обработкой дополнительных кнопок.

class LightChanderScene : public Scene {

public:
  LightChanderScene(Program &program, CameraMVP& cameraData,
                    std::vector<ILight *> lights = {},
                    std::vector<Mesh *> objects = {})
      : Scene(program, cameraData, lights, objects) {}

  /**
   * @brief Переключает состояние света
   *
   * @param i -- индекс источника света
   */
  void swapLight(size_t i) {
    if (i >= lights_.size()) {
      std::cout << "No light with this index, skip..." << std::endl;
      return;
    }
    lights_.at(i)->swapState();
  }
};

class LightChangerMoveHandler : public MoveEventHandler {
  LightChanderScene &scene_;
  GLint lastPressedkey_ = -1;

public:
  LightChangerMoveHandler(LightChanderScene &scene, CameraMVP &cameraData,
                          GLfloat moveSpeed = 0.05f,
                          GLfloat acceleration = 0.01f)
      : MoveEventHandler(cameraData, moveSpeed, acceleration), scene_(scene) {}

  void move() override {
    MoveEventHandler::move();
    if (lastPressedkey_ != -1 && pressed[lastPressedkey_])
      return;

    if (lastPressedkey_ != -1 && !pressed[lastPressedkey_])
      lastPressedkey_ = -1;

    if (pressed[GLFW_KEY_0]) {
      lastPressedkey_ = GLFW_KEY_0;
      scene_.swapLight(0);
    } else if (pressed[GLFW_KEY_1]) {
      lastPressedkey_ = GLFW_KEY_1;
      scene_.swapLight(1);
    } else if (pressed[GLFW_KEY_2]) {
      lastPressedkey_ = GLFW_KEY_2;
      scene_.swapLight(2);
    } else if (pressed[GLFW_KEY_3]) {
      lastPressedkey_ = GLFW_KEY_3;
      scene_.swapLight(3);
    } else if (pressed[GLFW_KEY_4]) {
      lastPressedkey_ = GLFW_KEY_4;
      scene_.swapLight(4);
    } else if (pressed[GLFW_KEY_5]) {
      lastPressedkey_ = GLFW_KEY_5;
      scene_.swapLight(5);
    } else if (pressed[GLFW_KEY_6]) {
      lastPressedkey_ = GLFW_KEY_6;
      scene_.swapLight(6);
    } else if (pressed[GLFW_KEY_7]) {
      lastPressedkey_ = GLFW_KEY_7;
      scene_.swapLight(7);
    } else if (pressed[GLFW_KEY_8]) {
      lastPressedkey_ = GLFW_KEY_8;
      scene_.swapLight(8);
    } else if (pressed[GLFW_KEY_9]) {
      lastPressedkey_ = GLFW_KEY_9;
      scene_.swapLight(9);
    }
  }
};

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
  Texture2D smileTex("./textures/scarySmile.png", 0);

  // Объекты
  Rectangle cube(glm::vec3(1), glm::vec3(0, .5, 0), blinPhongProgram,
                 containerTex);
  Rectangle floor(glm::vec3(50, 0.1, 50), glm::vec3(0, -1, 0),
                  blinPhongProgram, floorTex);
  Sphere sphere1(1, glm::vec3(7, 2, 7), blinPhongProgram, &containerTex, 100);
  Sphere sphere2(1, glm::vec3(9, 2, 7), blinPhongProgram, &containerTex, 100);
  Sphere sphere3(2, glm::vec3(8, 4, 7), blinPhongProgram, &containerTex, 3,
                 100);
  Sphere sphere4(1, glm::vec3(8, 6.5, 7), blinPhongProgram, &smileTex, 100);
  Rectangle hands(glm::vec3(8, 1, 1), glm::vec3(8, 4, 7), blinPhongProgram,
                  containerTex);
  Sphere sphere5(2, glm::vec3(16, 4, 7), blinPhongProgram, &containerTex, 100);
  Sphere sphere6(2, glm::vec3(0, 4, 7), blinPhongProgram, &containerTex, 100);
  Pyramid pyramid(glm::vec3(0, 2.5, 0), blinPhongProgram, &containerTex);
  glCheckError();

  // Камера
  CameraMVP cameraData(blinPhongProgram, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));


  // Источники света
  int LIGHTS_CNT = 2;
  glm::vec3 lightPos(10.0f, 15.0f, -1.0f);
  BlinPhongLight light1(lightPos, glm::vec3(0), cameraData, blinPhongProgram,
                        DepthFramebuffer(1200, 1200, 1));

  BlinPhongLight light2(lightPos * glm::vec3(-1, 1, 1), glm::vec3(0),
                        cameraData, blinPhongProgram,
                        DepthFramebuffer(1200, 1200, 1));

  // Сцены
  LightChanderScene scene(blinPhongProgram, cameraData, {&light1, &light2},
              {&cube, &floor, &sphere1, &sphere2, &sphere3, &sphere5, &sphere6,
               &sphere4, &pyramid, &hands});

  // Event Handler'ы
  LightChangerMoveHandler moveHandler(scene, cameraData);
  keyPressEvent += moveHandler;
  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  // Настройки шейдерной программы
  if (!blinPhongProgram.setUniformInt("main_texture0", 0))
    std::cerr << "Cant set main texture uniform!" << std::endl;
  if (!blinPhongProgram.setUniformInt("lightsCnt", LIGHTS_CNT))
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
    // sphere4.rotateAround(glm::vec3{0, 1, 0}, glm::radians(360.0f) * dt);

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
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  keyPressEvent.invoke(key, action, mods);
}

void mouseMoveCallback(GLFWwindow *win, double x, double y) {
  mouseMoveEvent.invoke(x, y);
}
