#include "Triangle2d.hpp"
#include <cstring>

Triangle2D::Triangle2D(GLfloat verticies[9], Program program,
                       void (*moveFN)(GLfloat*,const double & time,  const double & dt) )
    : Object(3, sizeof(GLfloat) * 3, verticies, program, moveFN) {
  verticies_ = new GLfloat[9];
  memcpy(verticies_, verticies, sizeof(GLfloat) * 9);
  program_.bind();
  geometry_.bindVAO();
  geometry_.bindVBO();

  setAttrPtr("pos", 3, sizeof(GLfloat) * 3, 0);

  geometry_.unbindVBO();
  geometry_.unbindVAO();

  program_.unbind();

  type_ = GL_TRIANGLES;
  draw();
}
