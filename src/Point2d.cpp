#include "Point2d.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include <cmath>
#include <cstring>

Point2D::Point2D(GLfloat vertex[3], Program program, void (*moveFN)(GLfloat*,const double & time,  const double & dt))
  : Object(1, sizeof(GLfloat)*3, vertex, program, moveFN)
  {
    verticies_ = new GLfloat[3];
    memcpy(verticies_, vertex, sizeof(GLfloat)*3);

    program_.bind();
    geometry_.bindVAO();
    geometry_.bindVBO();
    
    setAttrPtr("pos", 3, sizeof(GLint)*3, 0);

    geometry_.unbindVBO();
    geometry_.unbindVAO();

    program_.unbind();

    type_ = GL_POINTS;
    draw();
}



