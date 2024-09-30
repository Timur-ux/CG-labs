#include "Camera.hpp"


void Camera::update() {
  for(auto & [_, object] : objectsInFrame_) {
    object->draw(win_);
  }
}

Id Camera::getId() {
  return id;
}
