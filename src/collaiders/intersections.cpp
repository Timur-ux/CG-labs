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
#include "utils/printGlm.hpp"

glm::vec3 clampV(glm::vec3 x, glm::vec3 minVals, glm::vec3 maxVals) {
  x.x = glm::clamp(x.x, minVals.x, maxVals.x);
  x.y = glm::clamp(x.y, minVals.y, maxVals.y);
  x.z = glm::clamp(x.z, minVals.z, maxVals.z);

  return x;
}

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

  float diff2 = glm::dot(diff, diff);
  float rSum = a.r() + b.r();

  return diff2 < rSum * rSum;
}

bool isIntersect(AxisAlignedBB &a, Sphere &b) {
  glm::vec3 size2 = (a.max() - a.min()) / 2.0f;
  glm::vec3 a2b = b.center() - a.center();

  glm::vec3 closest = clampV(a2b, -size2, size2);

  if (closest == a2b) // В этом случае центр сферы внутри AABB
    return true;      // Чтобы сферу куда нибудь выкинуло из AABB

  glm::vec3 normal = a2b - closest;
  return glm::dot(normal, normal) < b.r() * b.r();
}

bool isIntersect(Sphere &a, AxisAlignedBB &b) { return isIntersect(b, a); }

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
  float yOverlap = aSize2.y + bSize2.y - glm::abs(a2b.y);
  float zOverlap = aSize2.z + bSize2.z - glm::abs(a2b.z);

  if (xOverlap <= 0 || yOverlap <= 0 || zOverlap <= 0) {
    std::cerr << "Warn: called collision calculating for non intersecting AABB "
                 "colliders, ret zeros\n";
    data.normal = glm::vec3(0);
    data.penetration = 0;
    return data;
  }

  if (xOverlap <= yOverlap && xOverlap <= zOverlap) {
    data.normal = glm::vec3(glm::sign(a2b.x), 0, 0);
    data.penetration = xOverlap;
  } else if (yOverlap <= xOverlap && yOverlap <= zOverlap) {
    data.normal = glm::vec3(0, glm::sign(a2b.y), 0);
    data.penetration = yOverlap;
  } else if (zOverlap <= xOverlap && zOverlap <= yOverlap) {
    data.normal = glm::vec3(0, 0, glm::sign(a2b.z));
    data.penetration = zOverlap;
  }

  return data;
}

CollisionData calculateIntersectionData(AxisAlignedBB &a, Sphere &b) {
  CollisionData data;

  glm::vec3 size2 = (a.max() - a.min()) / 2.0f;
  glm::vec3 a2b = b.center() - a.center();

  glm::vec3 closest = clampV(a2b, -size2, size2);

  if (glm::dot(closest - a2b, closest - a2b) < 0.01f) { // В этом случае центр сферы внутри AABB, выкинем её к верхней
             // грани ибо мне влом писать алгоритм поиска ближайшей грани
    std::cout << "Sphere into AABB\n";
    data.normal = glm::vec3(0, 1, 0);
    data.penetration = 0.1;
    return data;
  }

  glm::vec3 normal = a2b - closest;
  data.normal = glm::normalize(normal);
  data.penetration = b.r() - glm::length(normal);
  return data;
}

CollisionData calculateIntersectionData(Sphere &a, AxisAlignedBB &b) {
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
  RigidBody *aRigidBody =
      (RigidBody *)data.a->getComponent(engine::ComponentType::rigidBody);
  RigidBody *bRigidBody =
      (RigidBody *)data.b->getComponent(engine::ComponentType::rigidBody);

  if (!aRigidBody && !bRigidBody)
    return;

  if (!aRigidBody)
    aRigidBody = &nullRigidBody;
  if (!bRigidBody)
    bRigidBody = &nullRigidBody;

  glm::vec3 rv = bRigidBody->velocity() - aRigidBody->velocity();
  float velocityAlongNormal = glm::dot(rv, data.normal);

  if (velocityAlongNormal > 0)
    return;

  float e = glm::min(aRigidBody->restitution(), bRigidBody->restitution());

  float j = -(1 - e) * velocityAlongNormal /
            (aRigidBody->invMass() + bRigidBody->invMass()) ;
  
  // Потери энергии
  j *= 0.6;

  glm::vec3 impulse = j * data.normal;

  float massSum = aRigidBody->mass() + bRigidBody->mass();
  float ratio = aRigidBody->mass() / massSum;

  aRigidBody->setVelocity(aRigidBody->velocity() - ratio * impulse);
  ratio = 1 - ratio; // bRigidBody->mass / massSum;
  bRigidBody->setVelocity(bRigidBody->velocity() + ratio * impulse);

  // Positional correction

  constexpr float percent = 0.2;
  constexpr float slope = 0.01;
  glm::vec3 correction = glm::max(data.penetration - slope, 0.0f) /
                         (aRigidBody->invMass() + bRigidBody->invMass()) *
                         data.normal * percent;

  Transform *aTransform =
      (Transform *)data.a->getComponent(engine::ComponentType::transform);
  Transform *bTransform =
      (Transform *)data.b->getComponent(engine::ComponentType::transform);
  if (aRigidBody->host())
    aTransform->shiftBy(-aRigidBody->invMass() * correction);
  if (bRigidBody->host())
    bTransform->shiftBy(bRigidBody->invMass() * correction);
}

} // namespace collaider
