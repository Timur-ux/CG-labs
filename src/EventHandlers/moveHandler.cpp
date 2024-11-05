#include "EventHandlers/moveHandler.hpp"
#include <GLFW/glfw3.h>

#define max(a, b) (a > b ? a : b)

MoveEventHandler::MoveEventHandler(CameraMVP &cameraData, GLfloat moveSpeed, GLfloat acceleration) : cameraData_(cameraData)
      , moveSpeed_(moveSpeed)
      , acceleration_(acceleration){}


void MoveEventHandler::move() {
    if(pressed[GLFW_KEY_W]) 
      cameraData_.shiftBy(cameraData_.forward()*moveSpeed_);
    if(pressed[GLFW_KEY_S])
      cameraData_.shiftBy(-cameraData_.forward()*moveSpeed_);
    if(pressed[GLFW_KEY_A])
      cameraData_.shiftBy(glm::normalize(glm::cross(cameraData_.up(), cameraData_.forward()))*moveSpeed_);
    if(pressed[GLFW_KEY_D])
      cameraData_.shiftBy(-glm::normalize(glm::cross(cameraData_.up(), cameraData_.forward()))*moveSpeed_);
    if(pressed[GLFW_KEY_K])
      cameraData_.shiftBy(cameraData_.up()*moveSpeed_);
    if(pressed[GLFW_KEY_J])
      cameraData_.shiftBy(-cameraData_.up()*moveSpeed_);

    if(pressed[GLFW_KEY_KP_ADD])
      moveSpeed_ += acceleration_;
    if(pressed[GLFW_KEY_KP_SUBTRACT])
      moveSpeed_ = max(moveSpeed_ - acceleration_, 0);
  }

void MoveEventHandler::call(int key, int action, int mods) {
    if(key < 0 || key > 1023)
      return;
    if(action == GLFW_RELEASE) 
      pressed[key] = false;
    if(action == GLFW_PRESS)
      pressed[key] = true;
  }
