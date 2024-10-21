#include "CameraMVP.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "objects/Cube.hpp"
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

#define max(a, b) (a > b ? a : b)


void keyCallback(GLFWwindow *, int, int, int, int);
Event<int, int, int> keyPressEvent;

class MoveEventHandler : public IEventHandler<int, int, int> {
  CameraMVP &cameraData_;
  GLfloat moveSpeed_;
  GLfloat acceleration_;
  bool pressed[1024]{false};
public:
  MoveEventHandler(CameraMVP &cameraData, GLfloat moveSpeed = 0.05f, GLfloat acceleration = 0.05f)
      : cameraData_(cameraData)
      , moveSpeed_(moveSpeed)
      , acceleration_(acceleration){}

  void move() {
    if(pressed[GLFW_KEY_W]) 
      cameraData_.shiftBy(-cameraData_.forward()*moveSpeed_);
    if(pressed[GLFW_KEY_S])
      cameraData_.shiftBy(cameraData_.forward()*moveSpeed_);
    if(pressed[GLFW_KEY_A])
      cameraData_.shiftBy(-glm::normalize(glm::cross(cameraData_.up(), cameraData_.forward()))*moveSpeed_);
    if(pressed[GLFW_KEY_D])
      cameraData_.shiftBy(glm::normalize(glm::cross(cameraData_.up(), cameraData_.forward()))*moveSpeed_);
    if(pressed[GLFW_KEY_K])
      cameraData_.shiftBy(cameraData_.up()*moveSpeed_);
    if(pressed[GLFW_KEY_J])
      cameraData_.shiftBy(-cameraData_.up()*moveSpeed_);

    if(pressed[GLFW_KEY_KP_ADD])
      moveSpeed_ += acceleration_;
    if(pressed[GLFW_KEY_KP_SUBTRACT])
      moveSpeed_ = max(moveSpeed_ - acceleration_, 0);
  }

  void call(int key, int action, int mods) override {
    if(action == GLFW_RELEASE) 
      pressed[key] = false;
    if(action == GLFW_PRESS)
      pressed[key] = true;
  }
};

int main() {
  if (!glfwInit())
    std::cerr << "Error: GLFW initialization failed" << std::endl;
  glGetError();

  setDefaultCtxParams();
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  GLFWwindow *win = glfwCreateWindow(1200, 800, "Lab 2", NULL, NULL);

  glfwMakeContextCurrent(win);
  glfwSetKeyCallback(win, keyCallback);

  glCheckError();

  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

  Program program("./shaders/MVPShader.vsh",
                  "./shaders/texture2d.fsh");
  glCheckError();
  Texture2D texture("./textures/container.jpg");
  glCheckError();
  Cube cube(4, glm::vec3(0, 0, 0), program, texture);
  glCheckError();
  CameraMVP cameraData(program, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;

  glCheckError();
  Scene scene(program, cameraData, {&cube});
  glCheckError();
  glEnable(GL_DEPTH_TEST);
  glCheckError();
  double time = glfwGetTime(), prevTime = time;
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glCheckError();

  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    prevTime = time;
    time = glfwGetTime();
    float dt = time - prevTime;
    int width, height;

    glfwGetWindowSize(win, &width, &height);

    glClearColor(0, 0.2, 0.2, 1);

    glCheckError();
    scene.update(time, time - prevTime);
    glCheckError();

    moveHandler.move();

    glfwSwapBuffers(win);
    glViewport(0, 0, width, height);
  }
  glCheckError();

  glfwDestroyWindow(win);
  glfwTerminate();
  glCheckError();

  return 0;
}

void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
  keyPressEvent.invoke(key, action, mods);
}

