#ifndef MOVE_EVENT_HANDLER_HPP_
#define MOVE_EVENT_HANDLER_HPP_


#include "RigidBody.hpp"
#include "events.hpp"
#include "CameraMVP.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class MoveEventHandler : public IEventHandler<int, int, int> {
protected:
  MoveableBase &host_;
  GLfloat moveSpeed_;
  GLfloat acceleration_;
  bool pressed[1024]{false};

  RigidBody * rigidBody_ = nullptr;
  void moveRigidBody();
public:
  MoveEventHandler(MoveableBase &cameraData, GLfloat moveSpeed = 0.05f, GLfloat acceleration = 0.01f);
      
  void setRigidBody(RigidBody * rigidBody) {rigidBody_ = rigidBody;}
  virtual void move();
  void call(int key, int action, int mods) override;
};

#endif // !MOVE_EVENT_HANDLER_HPP_
