#ifndef TRIANGLE_2D_HPP_
#define TRIANGLE_2D_HPP_

#include "Program.hpp"
#include "Object.hpp"

class Triangle2D : public Object {
  public:
  Triangle2D(GLfloat verticies[9], Program program, void (*moveFN)(GLfloat*,const double & time, const double & dt) = nullptr);
};

#endif // !TRIANGLE_2D_HPP_
