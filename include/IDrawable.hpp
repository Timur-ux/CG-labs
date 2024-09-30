#ifndef IDRAWABLE_HPP_
#define IDRAWABLE_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IDrawable {
  public:
    virtual void draw(GLFWwindow * win) = 0;
};

#endif // !IDRAWABLE_HPP_
