// #define DEBUG

#include "CameraMVP.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "objects/Cube.hpp"
#include <complex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iterator>
#define GLEW_STATIC
#include "customWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef DEBUG
void printAnchor(glm::vec3 anchor, glm::mat4 model, glm::mat4 view,
                 glm::mat4 perspective) {
  glm::vec4 vertex(anchor, 1.0);

  glm::vec4 result =  view * model * vertex;

  std::cout << "{ " << result.x << ' ' << result.y << ' ' << result.z << " }"
            << std::endl;
}
#endif

int main() {
  if (!glfwInit())
    std::cerr << "Error: GLFW initialization failed" << std::endl;
  glGetError();

  setDefaultCtxParams();
  GLFWwindow *win = glfwCreateWindow(800, 600, "Lab 2", NULL, NULL);
  glfwMakeContextCurrent(win);

  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

  Program program("./shaders/modelConstructionShader.vsh",
                  "./shaders/redColor.fsh");
  Cube cube(1, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
            program);
  CameraMVP cameraData(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),
                       glm::vec3(0, 0, 0), 180);
  cameraData.lookAt({0, 0, 0});
  Scene scene(program, cameraData, {&cube});
  // glShadeModel(GL_FLAT);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);

  double time = glfwGetTime(), prevTime = time;
  cameraData.view_ = glm::translate(cameraData.view_, glm::vec3(0.0f, 0.0f, -3.0f));
  cameraData.perspective_ = glm::perspective(45.0f, (GLfloat)800.0 / (GLfloat)600.0, 0.1f, 100.0f);
  while (!glfwWindowShouldClose(win)) {
    glfwPollEvents();

    int width, height;

    glfwGetWindowSize(win, &width, &height);

    glClearColor(0, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prevTime = time;
    time = glfwGetTime();
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);

    cube.model_ = model;
    cameraData.view_ = view;
    cameraData.perspective_ = projection;
    
    scene.updateUniforms();
    scene.update(time, time - prevTime);

#ifdef DEBUG
    auto vertexData = cube.get();
    auto &model = cube.getModel();

    for (size_t i = 0; i < 8; ++i) {
      glm::vec3 anchor(vertexData[3 * i], vertexData[3 * i + 1],
                       vertexData[3 * i + 2]);

      std::cout << i << ") ";
      printAnchor(anchor, model, cameraData.view(), cameraData.perspective());
    }
    std::cout << "-----------------" << std::endl;
#endif

    glfwSwapBuffers(win);
    glViewport(0, 0, width, height);
  }

  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}
