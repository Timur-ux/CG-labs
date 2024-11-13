#ifndef RIGID_BODY_HPP_
#define RIGID_BODY_HPP_

#include "IMoveable.hpp"
#include "events.hpp"

constexpr float g = 9.83;
class RigidBody : public IEventHandler<double, double> {
  static constexpr glm::vec3 zero = glm::vec3(0);
  constexpr static float floorLevel_ = 1;
  constexpr static float velocityEps_ = 0.01;
  Transform *host_;
  float mass_;
  float invMass_;
  float mu_ = 0.8;
  float restitution_ = 0.8;
  glm::vec3 force_ = glm::vec3(0);

  glm::vec3 velocity_;

  void call(double time, double dt) override;

public:
  RigidBody(Transform *host, float mass, float mu = 0.8,
            float restitution = 0.5);
  ~RigidBody();

  void addForce(glm::vec3 force);
  void setVelocity(glm::vec3 newVelocity);

  const Transform * host() { return host_; }

  const glm::vec3 &velocity() const { return velocity_; }
  float restitution() const { return restitution_; }

  float mass() const { return mass_; }
  float invMass() const { return invMass_; }
};

#endif // !RIGID_BODY_HPP_
