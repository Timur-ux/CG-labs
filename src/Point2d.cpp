#include "Point2d.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include <cmath>
#include <cstring>

Point2D::Point2D(GLfloat vertex[3], Program program, TMoveFN moveFN)
    : Object(1, sizeof(GLfloat) * 3, vertex, program, moveFN) {
  type_ = GL_POINTS;
  draw();
}
