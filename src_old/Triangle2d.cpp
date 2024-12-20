#include "Triangle2d.hpp"
#include <cstring>

Triangle2D::Triangle2D(GLfloat verticies[9], Program program, TMoveFN moveFN)
    : Mesh(3, sizeof(GLfloat) * 3, verticies, program, moveFN) {
  type_ = GL_TRIANGLES;
  draw();
}
