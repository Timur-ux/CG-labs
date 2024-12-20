#ifndef PYRAMID_HPP_
#define PYRAMID_HPP_

#include "meshes/Mesh.hpp"
class Pyramid : public Mesh {
  glm::vec3 scale_;
  public:
  Pyramid(glm::vec3 position, Program & program, Texture2D * texture, glm::vec3 scale = glm::vec3(1));
};

#endif // !PYRAMID_HPP_
