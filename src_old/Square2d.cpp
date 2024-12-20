#include "Square2d.hpp"
#include "Object.hpp"
#include <GL/gl.h>

GLfloat * Square2d::constructSquarePoints(GLfloat center[2], GLfloat width, GLfloat height) {
  GLfloat * points = new GLfloat[4*3]{0};
  GLfloat w2 = width / 2, h2 = height / 2;

  points[0] = center[0] - w2;
  points[1] = center[1] - w2;

  points[3] = center[0] - w2;
  points[4] = center[1] + h2;

  points[6] = center[0] + w2;
  points[7] = center[1] + h2;

  points[9] = center[0] + w2;
  points[10] = center[1] - h2;

  return points;
}

void Square2d::updateSquarePoints(GLfloat center[2], GLfloat width, GLfloat height) {
  x_ = center[0];
  y_ = center[1];
  GLfloat w2 = width / 2, h2 = height / 2;

  verticies_[0] = center[0] - w2;
  verticies_[1] = center[1] - w2;

  verticies_[3] = center[0] - w2;
  verticies_[4] = center[1] + h2;

  verticies_[6] = center[0] + w2;
  verticies_[7] = center[1] + h2;

  verticies_[9] = center[0] + w2;
  verticies_[10] = center[1] - h2;

  geometry_.updateVerticies(verticies_);
}

Square2d::Square2d(GLfloat center[2], GLfloat width, GLfloat height, Program program, TMoveFN moveFN) : Mesh(4, sizeof(GLfloat) * 3, constructSquarePoints(center, width, height), program, moveFN), width_(width), height_(height) {
  x_ = center[0];
  y_ = center[1];
  type_ = GL_TRIANGLE_FAN;
  draw();
}
