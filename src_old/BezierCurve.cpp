#include "BezierCurve.hpp"
#include "Object.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

coord lerp(coord p1, coord p2, GLclampf t) {
  GLfloat x = p1.first + t*(p2.first - p1.first);
  GLfloat y = p1.second + t*(p2.second - p1.second);

  return {x, y};
} 

coord bezier(std::vector<coord> & coords, GLclampf t) {
  if(coords.size() == 2) return lerp(coords[0], coords[1], t);
  for(size_t i = 0; i < coords.size() - 1; ++i) {
    coords[i] = (lerp(coords[i], coords[i+1], t));
  }
  
  coords.pop_back();
  return bezier(coords, t);
}

coord BezierCurve2d::pointAt(GLclampf t) {
  std::vector<coord> temp(coords_);
  return bezier(temp, t);
}

GLfloat *BezierCurve2d::calculateVerticies(std::vector<coord> coords, GLuint vertexN) {
  if(coords.size() < 2) 
    throw std::invalid_argument("For building bezier curve at least 2 coords are needed");

  GLfloat * verticies = new GLfloat[(vertexN+1)*3];
  for(size_t i = 0; i < vertexN+1; ++i) {
    std::vector<coord> tempCoords(coords);
    GLclampf t = i*(1.0 / (GLfloat)vertexN);
    coord vertex = bezier(tempCoords, t);
    verticies[3*i] = vertex.first;
    verticies[3*i+1] = vertex.second;
    verticies[3*i+2] = 0;
  }

  return verticies;
}

BezierCurve2d::BezierCurve2d(std::vector<std::pair<GLfloat, GLfloat>> coords,
                             GLuint vertexN, Program program)
    : Mesh(vertexN+1, sizeof(GLfloat) * 3, calculateVerticies(coords, vertexN),
             program), coords_(coords) {
  type_ = GL_LINE_STRIP;
  draw();
}

void BezierCurve2d::draw() {
  Mesh::draw();
}

GLfloat * coordToGLfloatPtr(coord p) {
  static GLfloat res[2];
  res[0] = p.first;
  res[1] = p.second;
  return res;
}

SquareOnBezier2d::SquareOnBezier2d(GLfloat width, GLfloat height, Program program, BezierCurve2d & curve, void (*changeSizeFN)(const double &time, GLfloat&, GLfloat&))
  : Square2d(coordToGLfloatPtr(curve.pointAt(0)), width, height, program), curve_(curve), changeSizeFN_(changeSizeFN) {}

int sign(double val) {
  if(val >= 0) return 1;
  return -1;
}
void SquareOnBezier2d::call(const double & time, const double & dt) {
  t_ += direction_ * v_;
  if(t_ >= 1 ) {
    direction_ = -sign(direction_);
    t_ = 1;
  }
  if(t_ <= 0 ) {
    direction_ = -sign(direction_);
    t_ = 0;
  }
    if(changeSizeFN_) changeSizeFN_(time, width_, height_);

  updateSquarePoints(coordToGLfloatPtr(curve_.pointAt(t_)), width_, height_);
  
  updateEvent_.invoke(this);
}
