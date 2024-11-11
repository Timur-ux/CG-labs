#ifndef COLLAIDER_INTERSECTIONS_HPP_
#define COLLAIDER_INTERSECTIONS_HPP_

#include "IMoveable.hpp"
#include "collaiders/AABB.hpp"
#include "collaiders/Sphere.hpp"
namespace collaider {
  struct CollisionData {
    Transform * a;
    Transform * b;

    glm::vec3 normal;
    float penetration;
  };

  bool isIntersect(AxisAlignedBB & a, AxisAlignedBB & b);
  bool isIntersect(Sphere & a, Sphere & b);
  bool isIntersect(Sphere & a, AxisAlignedBB & b);
  bool isIntersect(AxisAlignedBB & a, Sphere & b);
}

#endif // !COLLAIDER_INTERSECTIONS_HPP_
