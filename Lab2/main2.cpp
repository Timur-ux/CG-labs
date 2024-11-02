#include "CameraMVP.hpp"
#include "Light/Lambert.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "events.hpp"
#include "glCheckError.hpp"
#include "objects/Rectangle.hpp"
#include "utils/printUniforms.hpp"
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
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

  Program program("./shaders/Lab2Shader3(Lambert,Simple).glsl");
  Program shadowMapProgram("./shaders/depthShader.glsl");
  eTB_GLSL__print_uniforms(program.get());
  eTB_GLSL__print_uniforms(shadowMapProgram.get());

  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glCheckError();
  glEnable(GL_DEPTH_TEST);
  glCheckError();
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(255);

 
  glCheckError();
  Texture2D containerTex("./textures/container.jpg", 0);
  Texture2D sunTex("./textures/sun3.png", 1);
  Texture2D depthTex(1200, 800, GL_DEPTH_COMPONENT,
                               {
                                   {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                   {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                   {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                                   {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
                                   {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
                               }, GL_FLOAT, 2);
  glCheckError();
  Rectangle cube(glm::vec3(1), glm::vec3(0, 0, 0), program, containerTex);
  Rectangle sun(glm::vec3(1), glm::vec3(5, 5, 5), program, sunTex);
  // Rectangle sun2(glm::vec3(4.1), glm::vec3(-30, 10, -20), program, sunTex);
  Rectangle floor(glm::vec3(500, 0.01, 500), glm::vec3(0, -1, -250), program, containerTex);
  glCheckError();
  CameraMVP cameraData(program, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0));

  MoveEventHandler moveHandler(cameraData);
  keyPressEvent += moveHandler;

  LookupEventHandler lookupHandler(cameraData, win);
  mouseMoveEvent += lookupHandler;

  glCheckError();
  double time = glfwGetTime(), prevTime = time;
  glCheckError();
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glCheckError();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex.texture(), 0);
  glCheckError();
  glDrawBuffer(GL_NONE);
  glCheckError();
  glReadBuffer(GL_NONE);
  glCheckError();
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer is incomplete!!!" << std::endl;
  }
  glCheckError();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glCheckError();

  int width, height;
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

    // render to depth map
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    depthTex.bind();
    glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.5f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0), glm::vec3(0, -1, 0));
    glm::mat4 lightSpace = proj * view;
    shadowMapProgram.bind();
    shadowMapProgram.setUniformMat4("lightSpaceMatrix", lightSpace);
    glClear(GL_DEPTH_BUFFER_BIT);
    floor.draw(&shadowMapProgram);
    cube.draw(&shadowMapProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // normal render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    proj =  cameraData.perspective();
    view = cameraData.view();
    program.setUniformMat4("perspective", proj);
    program.setUniformMat4("view", view);
    program.setUniformMat4("lightSpaceMatrix", lightSpace);
    glm::vec3 lightPos(5, 5, 5);
    program.setUniformVec3("lightPosition", lightPos);

    program.setUniformInt("shadowMap", depthTex.block());

    glm::mat3 normalMatrix = glm::inverseTranspose(cameraData.view()*floor.model());
    program.setUniformMat3("normalMatrix", normalMatrix);
    program.setUniformInt("diffuseTexture", containerTex.block());
    floor.draw();

    normalMatrix = glm::inverseTranspose(cameraData.view()*cube.model());
    program.setUniformMat3("normalMatrix", normalMatrix);
    program.setUniformInt("diffuseTexture", containerTex.block());
    cube.draw();

    normalMatrix = glm::inverseTranspose(cameraData.view()*sun.model());
    program.setUniformMat3("normalMatrix", normalMatrix);
    program.setUniformInt("diffuseTexture", sunTex.block());
    sun.draw();

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

