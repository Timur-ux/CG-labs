#ifndef MOVE_EVENT_HANDLER_HPP_
#define MOVE_EVENT_HANDLER_HPP_


#include "events.hpp"
#include "CameraMVP.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class MoveEventHandler : public IEventHandler<int, int, int> {
protected:
  CameraMVP &cameraData_;
  GLfloat moveSpeed_;
  GLfloat acceleration_;
  bool pressed[1024]{false};
public:
  MoveEventHandler(CameraMVP &cameraData, GLfloat moveSpeed = 0.05f, GLfloat acceleration = 0.01f);
      
  virtual void move();
  void call(int key, int action, int mods) override;
};

#endif // !MOVE_EVENT_HANDLER_HPP_
