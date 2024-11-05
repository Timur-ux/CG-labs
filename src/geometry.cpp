#include "geometry.hpp"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>

const static glm::vec3 kZero = glm::zero<glm::vec3>();

namespace geometry {

glm::mat4 constructTransform(glm::vec3 position, glm::vec3 forward, glm::vec3 up) {
  if(forward == kZero)
    throw std::invalid_argument("forward vector is zero vector");
  if(up == kZero)
    throw std::invalid_argument("up vector is zero vector");

  if(glm::dot(forward, up) == glm::length(forward)*glm::length(up)) 
    throw std::invalid_argument("Up and forward vectors are collinear");

  forward = glm::normalize(forward);
  up = glm::normalize(up);

  glm::vec3 right = glm::cross(forward, up);
  glm::vec3 up2 = glm::cross(right, forward);

  return glm::mat4(
      glm::vec4(right, 1.0),
      glm::vec4(up2, 1.0),
      glm::vec4(forward, 1.0),
      glm::vec4(-position, 1.0)
      );
}

} // !geometry
