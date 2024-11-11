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
  Transform & host_;
  double x_, y_;
  double yaw_ = -90, pitch_ = 0;
  double sensetivity_ = 0.05;
  GLFWwindow * win_;
  double pitchFov_ = 89.9;

  bool lockY_ = false;
  bool lockX_ = false;
  public:
  LookupEventHandler(Transform& host, GLFWwindow * win) : host_(host), win_(win) {
    glfwGetCursorPos(win, &x_, &y_);
  }

  void call(double newX, double newY) override; 
  void lockY() { lockY_ = !lockY_;}
  void lockX() { lockX_ = !lockX_;}

  void setPitchFov(double pitchFov) {
    pitchFov_ = pitchFov;
  }
  
  static bool first;
};

#endif // !LOOKUP_HANDLER_HPP_
