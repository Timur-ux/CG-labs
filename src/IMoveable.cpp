#include "IMoveable.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

MoveableBase::MoveableBase(glm::vec3 position, glm::vec3 target)
    : model_(1), position_(position),
      forward_(glm::normalize(target - position)) {}

void MoveableBase::shiftBy(glm::vec3 dp) {
  model_ = glm::translate(model_, dp);
  position_ += dp;
}

void MoveableBase::moveTo(glm::vec3 newPos) {
  model_ = glm::translate(model_, newPos - position_);
  position_ = newPos;
}

void MoveableBase::rotateAround(glm::vec3 v, float rads)  {
  glm::mat4 modelTemp(1);
  modelTemp = glm::rotate(modelTemp, rads, v);
  forward_ = glm::vec3(modelTemp * glm::vec4(forward_, 1.0f));
  up_ = glm::vec3(modelTemp * glm::vec4(up_, 1.0f));
  model_ = modelTemp * model_;
}
void MoveableBase::lookInto(glm::vec3 direction) {
  forward_ = glm::normalize(direction);
}
