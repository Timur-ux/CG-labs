#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "IUpdateable.hpp"
#include "Object.hpp"
#include <map>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Camera : public IUpdateable {
  Id id;
  GLFWwindow * win_;
  std::map<Id, ptr<Object>> objectsInFrame_;

  public:
  Camera(GLFWwindow * win) : win_(win) {}
  virtual void update() override;

  Id getId();
  virtual void recalcObjectsInFrame(std::vector<ptr<Object>> movedObjects) = 0;
};

#endif // !CAMERA_HPP_
