#include "collaiders/AABB.hpp"
#include "collaiders/CollaiderBase.hpp"

using namespace collaider;

AxisAlignedBB::AxisAlignedBB(float width, float height, float depth, Transform *host)
  : CollaiderBase(CollaiderType::AABB) {
  glm::vec3 diag2{width/2, height/2, depth/2};
  min_ = host->position() - diag2;
  max_ = host->position() + diag2;
  host_ = host;
}

AxisAlignedBB::AxisAlignedBB(float width, float height, float depth, glm::vec3 pos) 
  : CollaiderBase(CollaiderType::AABB) {
  glm::vec3 diag2{width/2, height/2, depth/2};
  min_ = pos - diag2;
  max_ = pos + diag2;
  host_ = nullptr;

}
