#ifndef RIGID_BODY_HPP_
#define RIGID_BODY_HPP_


#include "IMoveable.hpp"
#include "events.hpp"

constexpr float g = 9.83;
class RigidBody : public IEventHandler<double, double> {
  static constexpr glm::vec3 zero = glm::vec3(0);
  constexpr static float floorLevel_ = 1;
  constexpr static float velocityEps_ = 0.01;
  MoveableBase & host_;
  float mass_;
  float mu_ = 0.8; // before collaider done, set mu as constant
  glm::vec3 force_ = glm::vec3(0);

  glm::vec3 velocity_;

  void call(double time, double dt) override;
  public:

  RigidBody(MoveableBase &host, float mass);
  void addForce(glm::vec3 force);
  const glm::vec3 &velocity() {return velocity_;}


};

#endif // !RIGID_BODY_HPP_
