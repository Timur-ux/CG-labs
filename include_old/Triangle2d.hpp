#ifndef TRIANGLE_2D_HPP_
#define TRIANGLE_2D_HPP_

#include "Program.hpp"
#include "Object.hpp"

class Triangle2D : public Mesh {
  public:
  Triangle2D(GLfloat verticies[9], Program program, TMoveFN moveFN = nullptr);
};

#endif // !TRIANGLE_2D_HPP_
