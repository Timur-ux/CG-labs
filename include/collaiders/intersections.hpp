#ifndef COLLAIDER_INTERSECTIONS_HPP_
#define COLLAIDER_INTERSECTIONS_HPP_

#include "IMoveable.hpp"
#include "collaiders/AABB.hpp"
#include "collaiders/Sphere.hpp"
#include "Object.hpp"
#include <optional>

namespace collaider {
  struct CollisionData {
    engine::ObjectBase * a = nullptr;
    engine::ObjectBase * b = nullptr;

    glm::vec3 normal = glm::vec3(0);
    float penetration = -1;
  };

  bool isIntersect(AxisAlignedBB & a, AxisAlignedBB & b);
  bool isIntersect(Sphere & a, Sphere & b);
  bool isIntersect(Sphere & a, AxisAlignedBB & b);
  bool isIntersect(AxisAlignedBB & a, Sphere & b);

  bool isIntersect(engine::ObjectBase & a, engine::ObjectBase & b);

  std::optional<CollisionData> getIntersectionData(engine::ObjectBase *a, engine::ObjectBase * b);

  void resolveCollision(CollisionData data);
}

#endif // !COLLAIDER_INTERSECTIONS_HPP_
