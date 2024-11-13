#include "collaiders/intersections.hpp"
#include "IMoveable.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "collaiders/AABB.hpp"
#include "collaiders/CollaiderBase.hpp"
#include "collaiders/Sphere.hpp"
#include <glm/common.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>

namespace collaider {
bool isIntersect(AxisAlignedBB &a, AxisAlignedBB &b) {
  if (a.min().x > b.max().x || a.max().x < b.min().x)
    return false;
  if (a.min().y > b.max().y || a.max().y < b.min().y)
    return false;
  if (a.min().z > b.max().z || a.max().z < b.min().z)
    return false;

  return true;
}

bool isIntersect(Sphere &a, Sphere &b) {
  glm::vec3 diff = a.center() - b.center();
  float diff2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

  float rSum = a.r() + b.r();

  return diff2 < rSum * rSum;
}

glm::vec3 getClosestTo(AxisAlignedBB &a, const glm::vec3 &pos) {
  float direction[2]{1, -1};
  glm::vec3 diag2 = (a.max() - a.min()) / 2.0f;
  glm::vec3 bPos = (a.max() + a.min()) / 2.0f;

  float diffMin = 1e9;
  glm::vec3 res(0);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        glm::vec3 corner =
            bPos + diag2 * glm::vec3(direction[i], direction[j], direction[k]);
        glm::vec3 diff = corner - pos;

        float diff2 = glm::dot(diff, diff);
        if (diff2 < diffMin) {
          diffMin = diff2;
          res = corner;
        }
      }
    }
  }
  return res;
}
bool isIntersect(Sphere &a, AxisAlignedBB &b) {
  AxisAlignedBB aAABB(a.r(), a.r(), a.r(), a.host());
  if (!isIntersect(aAABB, b))
    return false;

  glm::vec3 closest = getClosestTo(b, a.center());
  glm::vec3 diff = closest - a.center();

  float diff2 = glm::dot(diff, diff);
  float r2 = a.r() * a.r();
  if (diff2 < r2)
    return true;

  return false;
}

bool isIntersect(AxisAlignedBB &a, Sphere &b) { return isIntersect(b, a); }

bool isIntersect(engine::ObjectBase &a, engine::ObjectBase &b) {
  CollaiderBase *aColliderBase =
      (CollaiderBase *)a.getComponent(engine::ComponentType::collaider);
  CollaiderBase *bColliderBase =
      (CollaiderBase *)b.getComponent(engine::ComponentType::collaider);
  if (!aColliderBase || !bColliderBase)
    return false;

  if (aColliderBase->type() == CollaiderType::AABB) {
    if (bColliderBase->type() == CollaiderType::AABB)
      return isIntersect(*(AxisAlignedBB *)aColliderBase,
                         *(AxisAlignedBB *)bColliderBase);
    else if (bColliderBase->type() == CollaiderType::Sphere)
      return isIntersect(*(AxisAlignedBB *)aColliderBase,
                         *(Sphere *)bColliderBase);
    else {
      std::cerr << "Undefined collider type detected, skip\n";
      return false;
    }
  } else if (aColliderBase->type() == CollaiderType::Sphere) {
    if (bColliderBase->type() == CollaiderType::AABB)
      return isIntersect(*(Sphere *)aColliderBase,
                         *(AxisAlignedBB *)bColliderBase);
    else if (bColliderBase->type() == CollaiderType::Sphere)
      return isIntersect(*(Sphere *)aColliderBase, *(Sphere *)bColliderBase);
    else {
      std::cerr << "Undefined collider type detected, skip\n";
      return false;
    }
  }
  std::cerr << "Undefined collider type detected, skip\n";
  return false;
}

CollisionData calculateIntersectionData(AxisAlignedBB &a, AxisAlignedBB &b) {
  CollisionData data;
  glm::vec3 aSize2 = (a.max() - a.min()) / 2.0f;
  glm::vec3 bSize2 = (b.max() - b.min()) / 2.0f;

  glm::vec3 a2b = b.center() - a.center();
  float xOverlap = aSize2.x + bSize2.x - glm::abs(a2b.x);
  float yOverlap = aSize2.x + bSize2.x - glm::abs(a2b.x);
  float zOverlap = aSize2.x + bSize2.x - glm::abs(a2b.x);

  if(xOverlap <= 0 || yOverlap <= 0 || zOverlap <= 0) {
    std::cerr << "Warn: called collision calculating for non intersecting AABB colliders, ret zeros\n" ;
    data.normal = glm::vec3(0);
    data.penetration = 0;
    return data;
  }

  if(xOverlap <= yOverlap && xOverlap <= zOverlap) {
    data.normal = glm::vec3(glm::sign(a2b.x), 0, 0);
    data.penetration = xOverlap;
  }
  else if(yOverlap <= xOverlap && yOverlap <= zOverlap) {
    data.normal = glm::vec3(0, glm::sign(a2b.y), 0);
    data.penetration = yOverlap;
  }
  else if(zOverlap <= xOverlap && zOverlap <= yOverlap) {
    data.normal = glm::vec3(0, 0, glm::sign(a2b.z));
    data.penetration = zOverlap;
  }


  return data;
}

CollisionData calculateIntersectionData(Sphere &a, AxisAlignedBB &b) {
  CollisionData data;
  glm::vec3 aPos = a.center();
  glm::vec3 closest = getClosestTo(b, a.center());

  data.normal = glm::normalize(aPos - closest);
  data.penetration = a.r() - glm::length(aPos - closest);

  return data;
}

CollisionData calculateIntersectionData(AxisAlignedBB &a, Sphere &b) {
  CollisionData data = calculateIntersectionData(b, a);
  data.normal *= -1;
  return data;
}

CollisionData calculateIntersectionData(Sphere &a, Sphere &b) {
  CollisionData data;
  glm::vec3 aPos = a.center();
  glm::vec3 bPos = b.center();

  data.normal = bPos - aPos;
  data.penetration = a.r() + b.r() - glm::length(data.normal);
  data.normal = glm::normalize(data.normal);

  return data;
}

std::optional<CollisionData> getIntersectionData(engine::ObjectBase *a,
                                                 engine::ObjectBase *b) {

  if (!a || !b || !isIntersect(*a, *b))
    return std::nullopt;

  CollisionData data{.a = a, .b = b};

  CollaiderBase *aColliderBase =
      (CollaiderBase *)a->getComponent(engine::ComponentType::collaider);
  CollaiderBase *bColliderBase =
      (CollaiderBase *)b->getComponent(engine::ComponentType::collaider);
  if (!aColliderBase || !bColliderBase)
    return std::nullopt;

  if (aColliderBase->type() == CollaiderType::AABB) {
    AxisAlignedBB *aCollider = (AxisAlignedBB *)aColliderBase;
    if (bColliderBase->type() == CollaiderType::AABB) {
      AxisAlignedBB *bCollider = (AxisAlignedBB *)bColliderBase;
      CollisionData data = calculateIntersectionData(*aCollider, *bCollider);
      data.a = a;
      data.b = b;
      return data;

    } else if (bColliderBase->type() == CollaiderType::Sphere) {
      Sphere *bCollider = (Sphere *)bColliderBase;
      CollisionData data = calculateIntersectionData(*aCollider, *bCollider);
      data.a = a;
      data.b = b;
      return data;
    } else {
      std::cerr << "Undefined collider type detected, skip\n";
      return std::nullopt;
    }
  } else if (aColliderBase->type() == CollaiderType::Sphere) {
    Sphere *aCollider = (Sphere *)aColliderBase;
    if (bColliderBase->type() == CollaiderType::AABB) {
      AxisAlignedBB *bCollider = (AxisAlignedBB *)bColliderBase;
      CollisionData data = calculateIntersectionData(*aCollider, *bCollider);
      data.a = a;
      data.b = b;
      return data;
    } else if (bColliderBase->type() == CollaiderType::Sphere) {
      Sphere *bCollider = (Sphere *)bColliderBase;
      CollisionData data = calculateIntersectionData(*aCollider, *bCollider);
      data.a = a;
      data.b = b;
      return data;
    } else {
      std::cerr << "Undefined collider type detected, skip\n";
      return std::nullopt;
    }
  }
  std::cerr << "Undefined collider type detected, skip\n";
  return std::nullopt;
}

static RigidBody nullRigidBody(nullptr, 0);
void resolveCollision(CollisionData data) {
  std::cout << "Resolving collision" << std::endl;
  RigidBody * aRigidBody = (RigidBody *)data.a->getComponent(engine::ComponentType::rigidBody);
  RigidBody * bRigidBody = (RigidBody *)data.b->getComponent(engine::ComponentType::rigidBody);

  if(!aRigidBody && !bRigidBody)
    return;

  if(!aRigidBody)
    aRigidBody = &nullRigidBody;
  if(!bRigidBody)
    bRigidBody = &nullRigidBody;

  glm::vec3 rv = bRigidBody->velocity() - aRigidBody->velocity();
  // data.normal = glm::vec3(0,1,0);
  float velocityAlongNormal = glm::dot(rv, data.normal);

  if(velocityAlongNormal < 0) 
    return;

  float e = glm::min(aRigidBody->restitution(), bRigidBody->restitution());

  float j = -(1 - e) * velocityAlongNormal / (aRigidBody->invMass() + bRigidBody->invMass());

  
  glm::vec3 impulse = j*data.normal;

  float massSum = aRigidBody->mass() + bRigidBody->mass();
  float ratio = aRigidBody->mass() / massSum;

  aRigidBody->setVelocity(aRigidBody->velocity() - ratio * impulse);
  ratio = 1 - ratio; // bRigidBody->mass / massSum;
  bRigidBody->setVelocity(bRigidBody->velocity() + ratio * impulse);


  // // Positional correction
  //
  // constexpr float percent = 0.2;
  // constexpr float slope = 0.01;
  // glm::vec3 correction = glm::max(data.penetration - slope, 0.0f) / (aRigidBody->invMass() + bRigidBody->invMass()) * data.normal;
  //
  // Transform * aTransform = (Transform *)data.a->getComponent(engine::ComponentType::transform);
  // Transform * bTransform = (Transform *)data.b->getComponent(engine::ComponentType::transform);
  // if(aRigidBody != &nullRigidBody) 
  //   aTransform->shiftBy(aRigidBody->invMass() * correction);
  // if(bRigidBody != &nullRigidBody)
  //   bTransform->shiftBy(bRigidBody->invMass()*correction);


}

} // namespace collaider
