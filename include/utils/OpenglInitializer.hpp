#ifndef OPENGL_INITIALIZER_HPP_
#define OPENGL_INITIALIZER_HPP_

#define GLEW_STATIC
#include "utils/customWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief MUST BE USED AS CONST STATIC VARIABLE
 * FOR SETUP OPENGL BEFORE USING IT FUNCTIONS
 * IN OTHER STATIC VARIABLES
 */

class OpenglInitializer {
private:
  GLFWwindow *window_ = nullptr;

public:
  OpenglInitializer();
  GLFWwindow *getWindow() { return window_; }
};

#endif // !OPENGL_INITIALIZER_HPP_
