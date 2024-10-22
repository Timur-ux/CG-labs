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
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;

class MoveEventHandler : public IEventHandler<int, int, int> {
  CameraMVP &cameraData_;
  GLfloat moveSpeed_;
  GLfloat acceleration_;
  bool pressed[1024]{false};
public:
  MoveEventHandler(CameraMVP &cameraData, GLfloat moveSpeed = 0.05f, GLfloat acceleration = 0.01f)
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
    if(key < 0 || key > 1023)
      return;
    if(action == GLFW_RELEASE) 
      pressed[key] = false;
    if(action == GLFW_PRESS)
      pressed[key] = true;
  }
};

class LookupEventHandler : public IEventHandler<double, double> {
  CameraMVP & cameraData_;
  double x_, y_;
  double yaw_ = -90, pitch_ = 0;
  double sensetivity_ = 0.05;
  bool first = false;
  GLFWwindow * win_;

  public:
  LookupEventHandler(CameraMVP& cameraData, GLFWwindow * win) : cameraData_(cameraData), win_(win) {
    glfwGetCursorPos(win, &x_, &y_);
  }

  void call(double newX, double newY) override {
    std::cout << newX << ' ' << newY << ' ' << std::endl;
    if(!first) {
      x_ = newX;
      y_ = newY;
      first = true;
      return;
    }

    double dx = newX - x_, dy = y_ - newY;
    x_ = newX, y_ = newY;

    dx *= sensetivity_;
    dy *= sensetivity_;

    yaw_ += dx;
    pitch_ += dy;
    if(pitch_ >= 89.0) pitch_ = 89.0;
    else if (pitch_ <= -89.0) pitch_ = -89.0;

    double yawRad = glm::radians(yaw_);
    double pitchRad = glm::radians(pitch_);
    
    glm::vec3 newForward;
    newForward.x = glm::cos(pitchRad) * glm::cos(yawRad);
    newForward.y = glm::sin(pitchRad);
    newForward.z = glm::cos(pitchRad) * glm::sin(yawRad);
    
    cameraData_.lookInto(newForward);
    glCheckError();

    glfwSetCursorPos(win_, 0, 0);
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
  glfwSetCursorPosCallback(win, mouseMoveCallback);

  glCheckError();

  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

  Program program("./shaders/MVPShader.vsh",
                  "./shaders/texture2d.fsh");
  glCheckError();
  Texture2D containerTex("./textures/container.jpg");
  Texture2D sunTex("./textures/sun3.png");
  glCheckError();
  Cube cube(4, glm::vec3(0, 0, 0), program, containerTex);
  Cube sun(4, glm::vec3(30, 10, -20), program, sunTex);
  glCheckError();
  CameraMVP cameraData(program, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;

  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  glCheckError();
  Scene scene(program, cameraData, {&cube, &sun});
  glCheckError();
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void mouseMoveCallback(GLFWwindow * win, double x, double y) {
  mouseMoveEvent.invoke(x, y);
}
