#include "collaiders/AABB.hpp"
#include "IMoveable.hpp"
#include "collaiders/CollaiderBase.hpp"

using namespace collaider;

AxisAlignedBB::AxisAlignedBB(float width, float height, float depth, Transform *host)
  : CollaiderBase(CollaiderType::AABB, host) {
  glm::vec3 diag2{width/2, height/2, depth/2};
  min_ = host->position() - diag2;
  max_ = host->position() + diag2;
  host_ = host;
  follow(host, glm::vec3(0));
}


void AxisAlignedBB::call() {
  glm::vec3 oldPos = (max_ + min_) / 2.0f;
  glm::vec3 dp = host_->position() - oldPos;

  max_ += dp;
  min_ += dp;
}
