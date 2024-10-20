#ifndef CAMERA_MVP_HPP_
#define CAMERA_MVP_HPP_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

struct CameraMVP {
  glm::vec3 position_;
  glm::vec3 up_;
  glm::vec3 target_;
  glm::mat4 view_;
  glm::mat4 perspective_;

  GLfloat ratio_;
  GLfloat fov_;
  GLfloat n = .1f, f = 100.0f;

  bool changed = false;

public:
  CameraMVP(glm::vec3 position, glm::vec3 up, glm::vec3 target,
            GLfloat fov = 45.0, GLfloat ratio = 4.0 / 3.0);

  const glm::mat4 &view() { return view_; };
  const glm::mat4 &perspective() { return perspective_; };

  void moveTo(glm::vec3 position);
  void lookAt(glm::vec3 target);

  void changeFov(GLfloat newFov);
  void changeRatio(GLfloat newRatio);

  bool isChanged() { return changed; }
  void setChangedOff() { changed = false; }
};

#endif // !CAMERA_MVP_HPP_