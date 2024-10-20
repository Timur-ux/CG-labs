
#include "CameraMVP.hpp"
#include "Program.hpp"
#include "Scene.hpp"
#include "glCheckError.hpp"
#include "objects/Cube.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLEW_STATIC
#include "customWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main() {
  if (!glfwInit())
    std::cerr << "Error: GLFW initialization failed" << std::endl;
  glGetError();

  setDefaultCtxParams();
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWwindow *win = glfwCreateWindow(800, 600, "Lab 2", NULL, NULL);
  glfwMakeContextCurrent(win);
  GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (GLEW_ARB_debug_output && flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
  glCheckError();
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
  glCheckError();
    glDebugMessageCallback(glDebugOutput, nullptr);
  glCheckError();
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glCheckError();
  }
  else
  {
    std::cout << "Can't load debug output extention" << std::endl;
  }

  glCheckError();

  glewExperimental = GL_TRUE;
  GLenum glewStatus;
  if ((glewStatus = glewInit()) != GLEW_OK)
    std::cerr << "Error: GLEW initialization failed, status code: "
              << glewStatus << std::endl;

  Program program("./shaders/modelConstructionShader.vsh",
                  "./shaders/redColor.fsh");
  glCheckError();
  Cube cube(4, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
            program);
  cube.model_ = glm::mat4(1);
  cube.model_ = glm::translate(cube.model_,  glm::vec3(0, 0, -10));
  cube.model_ = glm::rotate(cube.model_, glm::radians(30.f),  glm::vec3(1, 1, -1));
  glCheckError();
  CameraMVP cameraData(glm::vec3(0, 10, 10), glm::vec3(0, 1, 0),
                       glm::vec3(0, 2, 0));
  glCheckError();
  Scene scene(program, cameraData, {&cube});
  glCheckError();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  double time = glfwGetTime(), prevTime = time;
  glCheckError();
    glShadeModel(GL_FLAT);
  glCheckError();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glCheckError();
  while (!glfwWindowShouldClose(win)) {
    glfwPollEvents();

    int width, height;

    glfwGetWindowSize(win, &width, &height);

    glClearColor(0, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCheckError();
    prevTime = time;
    time = glfwGetTime();
    glCheckError();

  glCheckError();
    scene.updateUniforms();
  glCheckError();
    scene.update(time, time - prevTime);
  glCheckError();

    glfwSwapBuffers(win);
    glViewport(0, 0, width, height);
  }
  glCheckError();

  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}
