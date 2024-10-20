#include "CameraMVP.hpp"
#include "geometry.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <stdexcept>



CameraMVP::CameraMVP(glm::vec3 position, glm::vec3 up, glm::vec3 target,
      GLfloat fov, GLfloat ratio) 
  : position_(position)
  , up_(glm::normalize(up))
  , target_(target)
  , fov_(fov)
  , ratio_(ratio)
  , view_(1)
  , perspective_(1) {
  
    view_ = glm::lookAt(position_, target, up_);
    perspective_ = glm::perspective(glm::radians(fov_), ratio_, n, f);
  }


void CameraMVP::moveTo(glm::vec3 position) {
  position_ = position;

  view_ = glm::lookAt(position_, target_, up_);
  changed = true;
}

void CameraMVP::lookAt(glm::vec3 target) {
  target_ = target;
  view_ = glm::lookAt(position_, target_, up_);
  changed = true;
}

void CameraMVP::changeFov(GLfloat newFov) {
  if(newFov <= 0 || newFov > 180)
    throw std::invalid_argument("Fov must be in (0, 180]. Given: " + std::to_string(newFov));

  fov_ = newFov;
  perspective_ = glm::perspective(glm::radians(fov_), ratio_, n, f);
  changed = true;
}

void CameraMVP::changeRatio(GLfloat newRatio) {
  if(newRatio <= 0) 
    throw std::invalid_argument("Ratio must be positive. Given: " + std::to_string(newRatio));

  ratio_ = newRatio;
  perspective_ = glm::perspective(fov_, ratio_, 0.1f, 100.0f);
  changed = true;
}
