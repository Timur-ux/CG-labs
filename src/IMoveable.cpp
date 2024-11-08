#include "IMoveable.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

MoveableBase::MoveableBase(glm::vec3 position, glm::vec3 target)
    : translateModel_(1), rotateModel_(1), position_(position),
      forward_(glm::normalize(target - position)) {
  translateModel_ = glm::translate(translateModel_, position_);
}

void MoveableBase::shiftBy(glm::vec3 dp) {
  position_ += dp;
  translateModel_ = glm::translate(glm::mat4(1), position_);
  model_ = translateModel_ * rotateModel_;
}

void MoveableBase::moveTo(glm::vec3 newPos) {
  translateModel_ = glm::translate(translateModel_, newPos - position_);
  position_ = newPos;
  model_ = translateModel_ * rotateModel_;
}

void MoveableBase::scaleBy(glm::vec3 scale) {
  translateModel_ = glm::scale(translateModel_, scale);
  model_ = translateModel_ * rotateModel_;
}

void MoveableBase::rotateAround(glm::vec3 v, float rads) {
  glm::mat4 modelTemp(1);
  modelTemp = glm::rotate(modelTemp, rads, v);

  forward_ = glm::vec3(modelTemp * glm::vec4(forward_, 1.0f));
  up_ = glm::vec3(modelTemp * glm::vec4(up_, 1.0f));
  rotateModel_ = modelTemp * translateModel_;

  glm::vec3 oldPos = position_;
  position_ = modelTemp * glm::vec4(position_, 1);
  moveTo(oldPos);
}
void MoveableBase::lookInto(glm::vec3 direction) {
  forward_ = glm::normalize(direction);
}
