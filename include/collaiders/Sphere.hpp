#ifndef SPHERE_COLLIDER_HPP_
#define SPHERE_COLLIDER_HPP_

#include "IMoveable.hpp"
#include "collaiders/CollaiderBase.hpp"
namespace collaider {

class Sphere : public CollaiderBase, public Transform {
  glm::vec3 center_;
  float r_;
  Transform * host_;
public:
  Sphere(float r, Transform &host);

  float r() {return r_;}
  const glm::vec3 center() {return center_;}
};
} // !collaider

#endif // !SPHERE_COLLIDER_HPP_
