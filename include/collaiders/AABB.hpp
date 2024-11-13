#ifndef AABB_COLLAIDER_HPP_
#define AABB_COLLAIDER_HPP_

#include "IMoveable.hpp"
#include "collaiders/CollaiderBase.hpp"
#include <glm/glm.hpp>

namespace collaider {
class AxisAlignedBB : public CollaiderBase {
  glm::vec3 min_;
  glm::vec3 max_;

  void call() override;

public:
  AxisAlignedBB(float width, float height, float depth, Transform *host);
  const glm::vec3 &min() { return min_; }
  const glm::vec3 &max() { return max_; }
  glm::vec3 center() { return (max_ + min_) / 2.0f; }
};

} // namespace collaider

#endif //! AABB_COLLAIDER_HPP_
