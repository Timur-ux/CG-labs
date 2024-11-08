#include "EventHandlers/LookupHandler.hpp"
#include <cmath>
#include <glm/trigonometric.hpp>
#include <iostream>
#include "objects/Object.hpp"
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

bool LookupEventHandler::first = true;
void LookupEventHandler::call(double newX, double newY)  
{
    if(first) {
      x_ = newX, y_ = newY;
      first = false;
    }
    double dx = newX - x_, dy = y_ - newY;
    x_ = newX, y_ = newY;

    dx *= sensetivity_;
    dy *= sensetivity_;

    yaw_ += dx;
    pitch_ += dy;
    if(pitch_ >= 89.0) pitch_ = 89.0;
    else if (pitch_ <= -89.0) pitch_ = -89.0;

    double yawRad = glm::radians(yaw_);
    double pitchRad = glm::radians(pitch_);
    
    glm::vec3 newForward;
    newForward.x = glm::cos(pitchRad) * glm::cos(yawRad);
    newForward.y = glm::sin(pitchRad);
    newForward.z = glm::cos(pitchRad) * glm::sin(yawRad);
    
    cameraData_.lookInto(newForward);
    // std::cout << cameraData_.forward() << std::endl;
    glCheckError();


    int width, height;
  }
