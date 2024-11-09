#include "RigidBody.hpp"
#include "GlobalEvents.hpp"
#include "utils/printGlm.hpp"
#include <glm/geometric.hpp>
#include <iostream>

RigidBody::RigidBody(MoveableBase &host, float mass)
    : host_(host), mass_(mass) {
  velocity_ = glm::vec3(0);

  GlobalEvents::updateEvent += *this;
}

void RigidBody::call(double time, double dt) {
  // gravity
  force_ += glm::vec3(0, -mass_ * g, 0);

  // friction
  glm::vec3 horizontalVelocity = velocity_;
  horizontalVelocity.y = 0;
  glm::vec3 friction = glm::length(horizontalVelocity) > 0 ? -horizontalVelocity * mu_ /float(dt)* g : glm::vec3(0);

  if(velocity_ != zero)
    force_ += friction;

  glm::vec3 acceleration = force_ / mass_;


  if (velocity_ != zero && glm::length(velocity_) < velocityEps_) {
    velocity_ = zero;
  }

  velocity_ += acceleration * float(dt);

  host_.shiftBy(velocity_ * float(dt));
  glm::vec3 hostPos = host_.position();
  if (hostPos.y < floorLevel_) {
    hostPos.y = floorLevel_;
    host_.moveTo(hostPos);
    velocity_.y = 0;
  }

  force_ = zero;
}

void RigidBody::addForce(glm::vec3 force) { force_ += force; }
