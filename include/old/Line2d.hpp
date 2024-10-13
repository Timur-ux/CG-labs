#ifndef LINE_2D_HPP_
#define LINE_2D_HPP_

#include "Program.hpp"
#include "Object.hpp"

class Line2D : public Object {
public:
  Line2D(GLfloat verticies[6], Program program, TMoveFN moveFN = nullptr);
};

#endif // !LINE_2D_HPP_

