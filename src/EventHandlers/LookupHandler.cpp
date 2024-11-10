#include "EventHandlers/LookupHandler.hpp"
#include <cmath>
#include <glm/trigonometric.hpp>
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

    if(lockY_)
      dy = 0;
    if(lockX_)
      dx = 0;

    dx *= sensetivity_;
    dy *= sensetivity_;
    dx /= 2.0f;
    dy *= 2.0f;

    yaw_ += dx;
    pitch_ += dy;
    if(pitch_ >= pitchFov_) pitch_ = pitchFov_;
    else if (pitch_ <= -pitchFov_) pitch_ = -pitchFov_;

    double yawRad = glm::radians(yaw_);
    double pitchRad = glm::radians(pitch_);
    
    glm::vec3 newForward;
    newForward.x = glm::cos(pitchRad) * glm::cos(yawRad);
    newForward.y = glm::sin(pitchRad);
    newForward.z = glm::cos(pitchRad) * glm::sin(yawRad);
    
    host_.lookInto(host_.forward() + newForward);
    glCheckError();


    int width, height;
  }
