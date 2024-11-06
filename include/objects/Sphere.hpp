#ifndef SHPERE_HPP_
#define SHPERE_HPP_

#include "Object.hpp"

class Sphere : public Object {
  GLfloat r_;
  public:
  Sphere(GLfloat r, glm::vec3 position,  Program & program, Texture2D *texture, GLuint detalization = 100);
};


#endif // !SHPERE_HPP_
