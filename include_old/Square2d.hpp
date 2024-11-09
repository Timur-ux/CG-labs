#ifndef SQUARE_2D_HPP_
#define SQUARE_2D_HPP_

#include "Object.hpp"

class Square2d : public Mesh {
  protected:
    GLfloat width_, height_;
    GLfloat x_;
    GLfloat y_;
    static GLfloat * constructSquarePoints(GLfloat center[2], GLfloat width, GLfloat height);
  public:
    Square2d(GLfloat center[2], GLfloat width, GLfloat height, Program program, TMoveFN moveFN = nullptr);
    void updateSquarePoints(GLfloat center[2], GLfloat width, GLfloat height);

};

#endif // !SQUARE_2D_HPP_
