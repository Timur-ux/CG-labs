#include "CameraMVP.hpp"
#include "IMoveable.hpp"
#include "Light/BlinPhongLight.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "meshes/Rectangle.hpp"
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

void keyCallback(GLFWwindow *, int, int, int, int);
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;

/* Добавим в класс moveHandler пару кнопок для запоминания положения камеры и
 плавного перехода
 Добавленное управление:
 r - запомнить положение
 R - удалить последнее положение
 m - плавно переместиться к поседнему положению
 M - перестать плавно перемещаться к последнему положению
*/
class MoveAndCheckpointHandler : public MoveEventHandler {
  struct State {
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 forward;
  };
  std::vector<State> states_;
  bool isNowMovingToLastPos_ = false;
  GLint lastPressedKey = -1;

  constexpr static GLfloat eps_ = 0.05;

public:
  MoveAndCheckpointHandler(MoveableBase &host, GLfloat moveSpeed = 0.05f,
                           GLfloat acceleration = 0.01f)
      : MoveEventHandler(host, moveSpeed, acceleration) {}
  void move() override {
    MoveEventHandler::move();
    if(lastPressedKey != -1 && pressed[lastPressedKey]) 
      return;
    if(lastPressedKey != -1 && !pressed[lastPressedKey])
      lastPressedKey = -1;

    if (pressed[GLFW_KEY_R] && !pressed[GLFW_KEY_LEFT_SHIFT] &&
        !pressed[GLFW_KEY_RIGHT_SHIFT]) {
      lastPressedKey = GLFW_KEY_R;
      states_.push_back(
          {host_.position(), host_.up(), host_.forward()});
      std::cout << "Remember current state" << std::endl;
    }
    if (pressed[GLFW_KEY_R] &&
        (pressed[GLFW_KEY_LEFT_SHIFT] || pressed[GLFW_KEY_RIGHT_SHIFT])) {
      lastPressedKey = GLFW_KEY_R;
      if(states_.size() > 0)
        states_.pop_back();
      std::cout << "Dropping last remembered state" << std::endl;
    }
    if (pressed[GLFW_KEY_M] && !pressed[GLFW_KEY_LEFT_SHIFT] &&
        !pressed[GLFW_KEY_RIGHT_SHIFT]) {
      lastPressedKey = GLFW_KEY_M;
      isNowMovingToLastPos_ = true;
      std::cout << "Moving to last remembered state: Enabled" << std::endl;
    }
    else if (pressed[GLFW_KEY_M] &&
        (pressed[GLFW_KEY_LEFT_SHIFT] || pressed[GLFW_KEY_RIGHT_SHIFT])) {
      lastPressedKey = GLFW_KEY_M;
      isNowMovingToLastPos_ = false;
      std::cout << "Moving to last remembered state: Disabled" << std::endl;
    }

    // Обрабатываем перемещение, если включено
    if (isNowMovingToLastPos_) {
      if (states_.size() == 0) {
        isNowMovingToLastPos_ = false;
        std::cout << "No remembered states\n"
                  << "Moving to last remembered state: Disabled" << std::endl;
        return;
      }
      LookupEventHandler::first = true; // Поднимаем флаг первого передвижения мыши, чтобы избежать рывка курсора

      State curState(host_.position(), host_.up(),
                           host_.forward());
      State lastState = states_[states_.size() - 1];
      glm::vec3 dPos = lastState.position - curState.position;
      glm::vec3 dForward = lastState.forward - curState.forward;

      GLfloat posV = glm::length(dPos) > moveSpeed_ ? moveSpeed_ : glm::length(dPos);
      GLfloat forwardV = glm::length(dForward) > moveSpeed_ ? moveSpeed_ : glm::length(dForward);

      host_.shiftBy(dPos * posV);
      host_.lookInto(curState.forward + dForward * forwardV);

      if(glm::length(dPos) < eps_ && glm::length(dForward) < eps_) {
        isNowMovingToLastPos_ = false;
        std::cout << "At last remembered state now\n"
                  << "Moving to last remembered state: Disabled" << std::endl;
      }
    }
  }
};

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
  Rectangle cube(glm::vec3(1), glm::vec3(0, 1, 0), blinPhongProgram,
                 containerTex);
  Rectangle cube2(glm::vec3(.5), glm::vec3(5, 0, 5), blinPhongProgram,
                 containerTex, true);
  Rectangle floor(glm::vec3(25, 0.1, 25), glm::vec3(0, -1, 0),
                  blinPhongProgram, floorTex);
  glCheckError();

  // Камера
  CameraMVP cameraData(blinPhongProgram, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  // Event Handler'ы
  MoveAndCheckpointHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;
  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  // Источники света
  int LIGHTS_CNT = 3;
  glm::vec3 lightPos(10.0f, 5.0f, -1.0f);
  BlinPhongLight light1(lightPos, glm::vec3(0), cameraData, blinPhongProgram,
                        DepthFramebuffer(2400, 2400, 1));

  BlinPhongLight light2(lightPos + glm::vec3(3, -1, 3), glm::vec3(0),
                        cameraData, blinPhongProgram,
                        DepthFramebuffer(2400, 2400, 1));

  // Сцены
  Scene scene(blinPhongProgram, cameraData, {&light1, &light2},
              {&cube, &floor, &cube2});

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
