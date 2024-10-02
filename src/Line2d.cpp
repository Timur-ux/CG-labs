#include "Line2d.hpp"
#include "Object.hpp"
#include <cmath>
#include <cstring>
#include <iostream>

Line2D::Line2D(GLfloat verticies[6], Program program,
               void (*moveFN)(GLfloat*,const double & time,  const double & dt))
    : Object(2, sizeof(GLfloat) * 3, verticies, program, moveFN) {
  verticies_ = new GLfloat[6];
  memcpy(verticies_, verticies, sizeof(GLfloat) * 6);

  program_.bind();
  geometry_.bindVAO();
  geometry_.bindVBO();

  setAttrPtr("pos", 3, sizeof(GLfloat) * 3, 0);

  geometry_.unbindVBO();
  geometry_.unbindVAO();

  program_.unbind();

  type_ = GL_LINES;
  draw();
}
