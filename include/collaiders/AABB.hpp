#ifndef AABB_COLLAIDER_HPP_
#define AABB_COLLAIDER_HPP_

#include "IMoveable.hpp"
#include <glm/glm.hpp>

namespace collaider {

class AxisAlignedBB : private MoveableBase {
  float width_, height_;
  glm::vec3 position_;
  MoveableBase *host_;

public:
  AxisAlignedBB(float width, float height, MoveableBase *host)
      : width_(width), height_(height), host_(host) {
        follow(host_, {0, 0, 0});
        position_ = host_->position();
      }
};

} // namespace collaider

#endif //! AABB_COLLAIDER_HPP_
