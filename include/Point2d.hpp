#ifndef POINT_2D_HPP_
#define POINT_2D_HPP_

#include "Program.hpp"
#include "events.hpp"
#include "Object.hpp"

class Point2D : public Object {
public:
  Point2D(GLfloat vertex[3], Program program, void (*moveFN)(GLfloat*,const double & time,  const double & dt) = nullptr);
};

#endif // !POINT_2D_HPP_
