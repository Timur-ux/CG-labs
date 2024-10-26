#ifndef LOOKUP_HANDLER_HPP_
#define LOOKUP_HANDLER_HPP_

#include "events.hpp"
#include "CameraMVP.hpp"
#include "glCheckError.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



class LookupEventHandler : public IEventHandler<double, double> {
  CameraMVP & cameraData_;
  double x_, y_;
  double yaw_ = -90, pitch_ = 0;
  double sensetivity_ = 0.05;
  bool first = false;
  GLFWwindow * win_;

  public:
  LookupEventHandler(CameraMVP& cameraData, GLFWwindow * win) : cameraData_(cameraData), win_(win) {
    glfwGetCursorPos(win, &x_, &y_);
  }

  void call(double newX, double newY) override {
    if(!first) {
      x_ = newX;
      y_ = newY;
      first = true;
      return;
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
    glCheckError();


    int width, height;
    glfwGetWindowSize(win_, &width, &height);
    double w = width, h = height;



    if(newX < -w / 2 + 100) {
      newX = -w / 2 + 100;
    }
    else if(newX > w / 2 - 100) {
      newX = w / 2 - 100;
    }
    if(newY < -h / 2 + 100) {
      newY = -h / 2 + 100;
    }
    else if(newY > h / 2 - 100) {
      newY = h / 2 - 100;
    }

    glfwSetCursorPos(win_, newX, newY);
  }
};

#endif // !LOOKUP_HANDLER_HPP_
