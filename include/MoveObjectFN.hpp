#include "objects/Object.hpp"

class MoveObjectFN {
  Object * obj_;
  glm::vec3 center_;
  float speed_ = 0.001f;
  public:
  MoveObjectFN(Object * obj) : obj_(obj) {
    center_ = obj_->position() + obj_->forward();
  };

  void operator()(double time) {
    glm::vec3 newPos = center_ + glm::vec3(glm::sin(time), 0, glm::cos(time));
    obj_->shiftBy((newPos - obj_->position())*speed_);
  }
};
