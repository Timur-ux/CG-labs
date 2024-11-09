#include "Camera.hpp"
#include <GLFW/glfw3.h>


void Camera::update(const double &, const double&) {
  GLsizei width, height;
  
  glfwGetWindowSize(win_, &width, &height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(auto & [_, object] : objectsInFrame_) {
    object->draw();
  }

  glfwSwapBuffers(win_);
  glViewport(0, 0, width, height);
}

Id Camera::getId() {
  return id_;
}

void FullScene2DCamera::recalcObjectsInFrame(std::vector<Mesh*> movedObjects) {
  for(Mesh* object : movedObjects) {
    objectsInFrame_[object->getId()] = object;
  }
}
