#include "objects/Object.hpp"

// ИСПОЛЬЗУЕТСЯ ДЛЯ ДЕБАГГА СПОСОБНОСТИ ОБЪЕКТА К ПЕРЕМЕЩЕНИЮ

class MoveObjectFN {
  Object * obj_;
  glm::vec3 center_;
  float speed_ = 0.01f;
  public:
  MoveObjectFN(Object * obj) : obj_(obj) {
    center_ = obj_->position() + obj_->forward();
  };

  void operator()(double time) {
    glm::vec3 newPos = center_ + 4.0f*glm::vec3(glm::sin(time), glm::sin(time), glm::cos(time));
    obj_->shiftBy((newPos - obj_->position())*speed_);
  }
};
