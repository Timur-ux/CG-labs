#ifndef SHPERE_HPP_
#define SHPERE_HPP_

#include "meshes/Mesh.hpp"

class Sphere : public Mesh {
  GLfloat r_;
  public:
  Sphere(GLfloat r, glm::vec3 position,  Program & program, Texture2D *texture, GLuint detalization = 100);
  Sphere(GLfloat r, glm::vec3 position,  Program & program, Texture2D *texture, GLuint detalizationI, GLuint detalizationJ);

  GLfloat radius() {return r_;}
};


#endif // !SHPERE_HPP_
