#include "Line2d.hpp"
#include "Object.hpp"

Line2D::Line2D(GLfloat verticies[6], Program program, TMoveFN moveFN)
    : Object(2, sizeof(GLfloat) * 3, verticies, program, moveFN) {
  type_ = GL_LINES;
  draw();
}
