#include "RigidBody.hpp"
#include "GlobalEvents.hpp"
#include "utils/printGlm.hpp"
#include <glm/geometric.hpp>

RigidBody::RigidBody(Transform *host, float mass, float mu, float restitution)
    : host_(host), mass_(mass), mu_(mu), restitution_(restitution), velocity_(0) {
  if(mass_ == 0)
    invMass_ = 0;
  else
    invMass_ = 1/mass_;

  GlobalEvents::updateEvent += *this;
}

RigidBody::~RigidBody() {
  GlobalEvents::updateEvent -= *this;
}

void RigidBody::call(double time, double dt) {
  if(!host_)
    return;
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

  host_->shiftBy(velocity_ * float(dt));
  // glm::vec3 hostPos = host_->position();
  // if (hostPos.y < floorLevel_) {
  //   hostPos.y = floorLevel_;
  //   host_->moveTo(hostPos);
  //   velocity_.y = 0;
  // }

  force_ = zero;
}

void RigidBody::addForce(glm::vec3 force) { force_ += force; }
