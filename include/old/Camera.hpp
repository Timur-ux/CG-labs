#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "IUpdateable.hpp"
#include "Object.hpp"
#include <cstdlib>
#include <map>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera : public IUpdateable {
  Id id_;

  protected:
  GLFWwindow * win_;
  std::map<Id, Object*> objectsInFrame_;

  public:
  Camera(GLFWwindow * win) : win_(win), id_(rand()) {}
  virtual void update(const double &, const double &) override;

  Id getId();
  virtual void recalcObjectsInFrame(std::vector<Object*> movedObjects) = 0;
};

class FullScene2DCamera : public Camera {
public:
  FullScene2DCamera(GLFWwindow * win) : Camera(win) {};
  void recalcObjectsInFrame(std::vector<Object*> movedObjects) override final;
};

#endif // !CAMERA_HPP_
