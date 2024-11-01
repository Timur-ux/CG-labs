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
#include "Framebuffer.hpp"


void keyCallback(GLFWwindow *, int, int, int, int);
void mouseMoveCallback(GLFWwindow *, double, double);
Event<int, int, int> keyPressEvent;
Event<double, double> mouseMoveEvent;



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

  Program program("./shaders/Lab2Shader1(Lambert).glsl");
  eTB_GLSL__print_uniforms(program.get());
  // Program program("./shaders/simpleShader.vsh", "./shaders/redColor.fsh");

  int width, height;
  glfwGetWindowSize(win, &width, &height);

 
  glCheckError();
  Texture2D containerTex("./textures/container.jpg", 0);
  Texture2D sunTex("./textures/sun3.png", 1);
  glCheckError();
  Rectangle cube(glm::vec3(1), glm::vec3(0, 0, 0), program, containerTex);
  Rectangle sun(glm::vec3(1), glm::vec3(1, 1, -5), program, sunTex);
  // Rectangle sun2(glm::vec3(4.1), glm::vec3(-30, 10, -20), program, sunTex);
  Rectangle floor(glm::vec3(500, 0.01, 500), glm::vec3(0, -1, -250), program, containerTex);
  glCheckError();
  CameraMVP cameraData(program, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;

  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  LambertLight light(&sun, cameraData, program, Framebuffer::createShadowMap(512, 512, 2));
  // LambertLight light2(&sun2, cameraData, program, Framebuffer::createShadowMap(512, 512, 3));

  glCheckError();
  // Scene scene(program, cameraData, {&light, &light2}, {&cube, &sun, &sun2, &floor});
  Scene scene(program, cameraData, {&light}, {&cube, &floor});
  glCheckError();
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glCheckError();
  glEnable(GL_DEPTH_TEST);
  glCheckError();
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(255);
  glCheckError();
  double time = glfwGetTime(), prevTime = time;
  glCheckError();

  while (!glfwWindowShouldClose(win)) {
    glCheckError();

    glfwGetWindowSize(win, &width, &height);
    cameraData.updateState();
    glClearColor(0, 0.2, 0.2, 1);
    glfwPollEvents();
    moveHandler.move();
    prevTime = time;
    time = glfwGetTime();
    float dt = time - prevTime;
    scene.update(time, dt);


    glViewport(0, 0, width, height);
    glfwSwapBuffers(win);
  }
  glCheckError();

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
