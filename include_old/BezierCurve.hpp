#ifndef BEZIER_CURVE_2D_HPP_
#define BEZIER_CURVE_2D_HPP_

#include "Object.hpp"
#include "Program.hpp"
#include "Square2d.hpp"
#include "events.hpp"
#include <GL/gl.h>
#include <vector>


using coord = std::pair<GLfloat, GLfloat>;
class BezierCurve2d : public Mesh {
  std::vector<coord> coords_;

  GLfloat * calculateVerticies(std::vector<std::pair<GLfloat, GLfloat>> coords, GLuint vertexN);
public:
  BezierCurve2d(std::vector<std::pair<GLfloat, GLfloat>> anchors, GLuint vertexN, Program program);
  void draw() override;

  coord pointAt(GLclampf t);
};

class SquareOnBezier2d : public Square2d {
  BezierCurve2d & curve_;
  double v_ = 0.0005;
  GLclampf t_ = 0;
  int direction_ = 1;
  void (*changeSizeFN_)(const double & time, GLfloat&, GLfloat&);
  public:
  SquareOnBezier2d(GLfloat width, GLfloat height, Program program, BezierCurve2d & curve, void (*changeSizeFN)(const double &time, GLfloat&, GLfloat&) = nullptr);
  void call(const double & time, const double &dt);

  void changeSpeed(double dv) {
    v_ += dv;
  }

  double getV() {
    return v_;
  }

};
#endif // !BEZIER_CURVE_2D_HPP_
