#include "IMoveable.hpp"
#include "utils/printGlm.hpp"
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

MoveableBase::MoveableBase(glm::vec3 position, glm::vec3 forward)
    : translateModel_(1), rotateModel_(1), position_(position),
      forward_(forward), onMove(onMove_) {
  translateModel_ = glm::translate(translateModel_, position_);
  model_ = translateModel_ * rotateModel_;
}

void MoveableBase::shiftBy(glm::vec3 dp) {
  // std::cout << dp;
  position_ += dp;
  translateModel_ = glm::translate(glm::mat4(1), position_);
  model_ = translateModel_ * rotateModel_;
  onMove_.invoke();
}

void MoveableBase::moveTo(glm::vec3 newPos) {
  translateModel_ = glm::translate(translateModel_, newPos - position_);
  position_ = newPos;
  model_ = translateModel_ * rotateModel_;
  onMove_.invoke();
}

void MoveableBase::scaleBy(glm::vec3 scale) {
  translateModel_ = glm::scale(translateModel_, scale);
  model_ = translateModel_ * rotateModel_;
  onMove_.invoke();
}

void MoveableBase::rotateAround(glm::vec3 v, float rads) {
  glm::mat4 modelTemp(1);
  modelTemp = glm::rotate(modelTemp, rads, v);

  forward_ = glm::vec3(modelTemp * glm::vec4(forward_, 1.0f));
  rotateModel_ = modelTemp * rotateModel_;

  glm::vec3 oldPos = position_;
  position_ = modelTemp * glm::vec4(position_, 1);
  moveTo(oldPos);
  onMove_.invoke();
}

void MoveableBase::lookInto(glm::vec3 direction) {
  direction = glm::normalize(direction);
  if (direction == forward_ || direction == -forward_) {
    std::cerr << "Undefined rotation, skip" << std::endl;
    return;
  }
  if(glm::abs(glm::dot(forward_, direction) - 1) < 0.000001 ) {
    std::cerr << "Undefined rotation, skip" << std::endl;
    return;
  }

  glm::vec3 n = glm::cross(forward_, direction);
  float angle = glm::acos(glm::dot(forward_, direction));
  std::cout << n << angle << ' ' <<glm::dot(forward_, direction)<< std::endl;
  ;
  // angle = glm::min(angle, glm::pi<float>() - angle);

  // rotateModel_ = glm::rotate(rotateModel_, angle, n);
  glm::mat4 rotMat = glm::rotate(glm::mat4(1), angle, n);
  rotateModel_ = rotMat * rotateModel_;
  model_ = translateModel_ * rotateModel_;

  forward_ = rotMat * glm::vec4(forward_, 0);
  // up_ = glm::vec3(rotMat * glm::vec4(up_, 0));
  onMove_.invoke();
}

// void MoveableBase::lookInto(glm::vec3 direction) {
//   direction = glm::normalize(direction);
//   if(direction == forward_ || direction == -forward_) {
//     std::cerr << "Undefined rotation, skip" << std::endl;
//     return;
//   }
//
//   glm::vec4 left = glm::vec4(glm::cross(up_, direction), 0);
//   glm::vec4 up;
//   if(upFreezed_)
//     up = glm::vec4(up_, 0);
//   else
//    up = glm::vec4(glm::cross(direction, glm::vec3(left)), 0);
//   glm::mat4 rotMat(left, up, glm::vec4(direction, 0), {0,0,0,1});
//
//   rotateModel_ = rotMat;
//   model_ = translateModel_ * rotateModel_;
//
//   forward_ = direction;
//   up_ = glm::vec3(up);
//   onMove_.invoke();
// }

void MoveableBase::lookAt(glm::vec3 position) {
  glm::vec3 direction = glm::normalize(position - position_);
  forward_ = direction;

  // lookInto(direction);
}

void MoveableBase::follow(MoveableBase *objToFollow, glm::vec3 followOffset) {
  if (following_ != nullptr)
    following_->onMove -= *this;

  objToFollow->onMove += *this;
  following_ = objToFollow;
  followOffset_ = followOffset;
}

void MoveableBase::call() {
  glm::vec3 offset =
      (following_->up() - following_->forward()) * glm::length(followOffset_);

  moveTo(following_->position() + offset);
  // lookAt(following_->position() - offset);
}
