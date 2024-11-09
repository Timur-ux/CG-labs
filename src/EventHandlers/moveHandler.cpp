#include "EventHandlers/moveHandler.hpp"
#include "RigidBody.hpp"
#include "utils/printGlm.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#define max(a, b) (a > b ? a : b)

MoveEventHandler::MoveEventHandler(MoveableBase &cameraData, GLfloat moveSpeed,
                                   GLfloat acceleration)
    : host_(cameraData), moveSpeed_(moveSpeed), acceleration_(acceleration) {}

void MoveEventHandler::move() {
  if (pressed[GLFW_KEY_KP_ADD])
    moveSpeed_ += acceleration_;
  if (pressed[GLFW_KEY_KP_SUBTRACT])
    moveSpeed_ = max(moveSpeed_ - acceleration_, 0);

  if (rigidBody_) {
    moveRigidBody();
    return;
  }

  if (pressed[GLFW_KEY_W]) {
    glm::vec3 dp = host_.forward() * moveSpeed_;
    // std::cout << host_.forward() << dp;
    host_.shiftBy(dp);
  }
  if (pressed[GLFW_KEY_S]) {
    glm::vec3 dp = -host_.forward() * moveSpeed_;
    host_.shiftBy(dp);
  }
  if (pressed[GLFW_KEY_A]) {
    // std::cout << host_.up() << host_.forward();
    glm::vec3 dp =
        glm::normalize(glm::cross(host_.up(), host_.forward())) * moveSpeed_;
    host_.shiftBy(dp);
  }
  if (pressed[GLFW_KEY_D]) {
    // std::cout << host_.up() << host_.forward();
    glm::vec3 dp =
        -glm::normalize(glm::cross(host_.up(), host_.forward())) * moveSpeed_;
    host_.shiftBy(dp);
  }
  if (pressed[GLFW_KEY_K]) {
    glm::vec3 dp = host_.up() * moveSpeed_;
    host_.shiftBy(dp);
  }
  if (pressed[GLFW_KEY_J]) {
    glm::vec3 dp = -host_.up() * moveSpeed_;
    host_.shiftBy(dp);
  }
}

void MoveEventHandler::moveRigidBody() {
  if (pressed[GLFW_KEY_W])
    rigidBody_->addForce(host_.forward() * moveSpeed_);
  if (pressed[GLFW_KEY_S])
    rigidBody_->addForce(-host_.forward() * moveSpeed_);
  if (pressed[GLFW_KEY_A])
    rigidBody_->addForce(
        glm::normalize(glm::cross(host_.up(), host_.forward())) * moveSpeed_);
  if (pressed[GLFW_KEY_D])
    rigidBody_->addForce(
        -glm::normalize(glm::cross(host_.up(), host_.forward())) * moveSpeed_);
  if (pressed[GLFW_KEY_K])
    rigidBody_->addForce(host_.up() * moveSpeed_);
  if (pressed[GLFW_KEY_J])
    rigidBody_->addForce(-host_.up() * moveSpeed_);
}

void MoveEventHandler::call(int key, int action, int mods) {
  if (key < 0 || key > 1023)
    return;
  if (action == GLFW_RELEASE)
    pressed[key] = false;
  if (action == GLFW_PRESS)
    pressed[key] = true;
}
