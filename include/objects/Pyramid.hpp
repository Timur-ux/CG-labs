#ifndef PYRAMID_HPP_
#define PYRAMID_HPP_

#include "objects/Object.hpp"
class Pyramid : public Object {
  glm::vec3 scale_;
  public:
  Pyramid(glm::vec3 position, Program & program, Texture2D * texture, glm::vec3 scale = glm::vec3(1));
};

#endif // !PYRAMID_HPP_
