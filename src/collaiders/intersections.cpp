#include "collaiders/intersections.hpp"

namespace collaider {
bool isIntersect(AxisAlignedBB & a, AxisAlignedBB & b) {
  if(a.min().x > b.max().x || a.max().x < b.min().x) return false;
  if(a.min().y > b.max().y || a.max().y < b.min().y) return false;
  if(a.min().z > b.max().z || a.max().z < b.min().z) return false;

  return true;
}


bool isIntersect(Sphere & a, Sphere & b) {
  glm::vec3 diff = a.center() - b.center();
  float diff2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

  float rSum = a.r() + b.r();

  return diff2 < rSum * rSum;
}

bool isIntersect(Sphere & a, AxisAlignedBB & b) {
  AxisAlignedBB aAABB(a.r(), a.r(), a.r(), a.center());
  if(!isIntersect(aAABB, b)) 
    return false;

  float direction[2] {1, -1};
  glm::vec3 diag2 = (b.max() - b.min()) / 2.0f;
  glm::vec3 bPos = (b.max() + b.min()) / 2.0f;

  for(int i = 0; i < 2; ++i) {
    for(int j = 0; j < 2; ++j) {
      for(int k = 0; k < 2; ++k) {
        glm::vec3 corner = bPos + diag2*glm::vec3(direction[i], direction[j], direction[k]);
        glm::vec3 diff = corner - a.center();

        float diff2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        float r2 = a.r() * a.r();
        if(diff2 < r2)
          return true;
      }
    }
  }

  return false;
}

bool isIntersect(AxisAlignedBB & a, Sphere & b) {
  return isIntersect(b, a);
}

}



