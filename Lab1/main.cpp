#include <cmath>
#include <cstdlib>
#include <iostream>
#include "BezierCurve.hpp"
#include "Program.hpp"
#include "button.hpp"
#include "customWindow.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "events.hpp"
#include <GLFW/glfw3.h>
#include <memory>

Event<GLfloat, GLfloat> clickOnWindowEvent;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouseClickCallback(GLFWwindow * window, int button, int action, int mode) {
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double x, y;
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &x, &y);
    clickOnWindowEvent.invoke(2*x/width - 1, 1 - 2*y / height);
  }
};

class SpeedChangeEventHandler : public IEventHandler<> {
  Button& button_;
  SquareOnBezier2d& square_;
  double dv_;
  public:
  SpeedChangeEventHandler(Button & button, SquareOnBezier2d & square, double dv) : button_(button), square_(square), dv_(dv) {
    button_.onClick += *this;
  };

  ~SpeedChangeEventHandler() {
    button_.onClick -= *this;
  }

  void call() override {
    square_.changeSpeed(dv_);
    std::cout << "Change speed by: " << dv_ << "\nCurrent speed: " << square_.getV() << std::endl;
  }
};


void squareChangeSize(const double & time, GLfloat & width, GLfloat & height) {
  width = sin(time)/5;
  height = cos(time)/5;
}

int main() {

  glfwInit(); // Инициализируем glfw
  glGetError(); // Вызов glfwInit может привести к ошибке, игнорируем её


  setDefaultCtxParams();
  Scene scene;

  GLFWwindow * win = glfwCreateWindow(800, 600, "Lab 1", NULL, NULL);
  glfwMakeContextCurrent(win); // Создаем контекст OpenGL и делаем его текущим
  glfwSetKeyCallback(win, keyCallback);
  glfwSetMouseButtonCallback(win, mouseClickCallback);

  glewExperimental = GL_TRUE; // ТОЛЬКО после этого инициализируем glew
  GLenum glewStatus;
  if((glewStatus = glewInit()) != GLEW_OK) {
    std::cout << "Error: glew not initialized, err code: " << glewStatus << std::endl;
  }

  Program blueColorProgram = Program("./shaders/simpleShader.vsh", "./shaders/blueColor.fsh");
  Program redColorProgram = Program("./shaders/simpleShader.vsh", "./shaders/redColor.fsh");
  Program greenColorProgram = Program("./shaders/simpleShader.vsh", "./shaders/greenColor.fsh");

  std::vector<coord> bezierCoords {
    {0, 0},
    {-0.5, -0.5},
    {0.5, -0.5},
    {0, 0},
    {-1, 1},
    {1, 1},
    {0, 0},
  };

  GLfloat squareCenter[2] {0, 0};

  BezierCurve2d bezier(bezierCoords, 50, blueColorProgram);
  SquareOnBezier2d square(0.1, 0.1, blueColorProgram, bezier, squareChangeSize);
  Button speedUpButton(std::shared_ptr<GLfloat>(new GLfloat[2]{0.6, 0.8}).get(), 0.1, 0.1, greenColorProgram, clickOnWindowEvent);
  Button speedDownButton(std::shared_ptr<GLfloat>(new GLfloat[2]{0.4, 0.8}).get(), 0.1, 0.1, redColorProgram, clickOnWindowEvent);

  double dv = 0.001;
  SpeedChangeEventHandler speedUpHandler(speedUpButton, square, dv);
  SpeedChangeEventHandler speedDownHandler(speedDownButton, square, -dv);

  scene.addObject(&bezier);
  scene.addObject(&square);
  scene.addObject(&speedUpButton);
  scene.addObject(&speedDownButton);

  ptr<FullScene2DCamera> camera = std::make_shared<FullScene2DCamera>(win);

  scene.addCamera(camera.get());

  double time = glfwGetTime();
  double prevTime = time;
  double dt = dt;

  scene.initialUpdate(dt);
  while(!glfwWindowShouldClose(win)) {
    time = glfwGetTime();
    dt = time - prevTime;
    prevTime = time;
    glfwPollEvents();

    scene.update(time, dt);
  }
  
  
  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}


