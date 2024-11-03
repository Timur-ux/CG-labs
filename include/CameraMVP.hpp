#ifndef CAMERA_MVP_HPP_
#define CAMERA_MVP_HPP_

#include "IMoveable.hpp"
#include "Program.hpp"
#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

struct CameraMVP : public MoveableBase {
  glm::mat4 view_;
  glm::mat4 perspective_;

  GLfloat ratio_;
  GLfloat fov_;
  GLfloat n = .1f, f = 100.0f;

  Program & program_;

  GLint viewLoc_;
  GLint perspectiveLoc_;

  GLboolean viewChanged_ = false;
  GLboolean perspectiveChanged_ = false;

public:
  CameraMVP(Program & program, glm::vec3 position, glm::vec3 up, glm::vec3 target,
            GLfloat fov = 45.0, GLfloat ratio = 4.0 / 3.0);

  const glm::mat4 &view() const { return view_; };
  const glm::mat4 &perspective() const { return perspective_; };

  void lookAt(glm::vec3 target);
  void lookInto(glm::vec3 direction);

  void changeFov(GLfloat newFov);
  void changeRatio(GLfloat newRatio);

  virtual void shiftBy(glm::vec3 dp) override;
  virtual void moveTo(glm::vec3 newPosition) override;
  virtual void rotateAround(glm::vec3 v, float rads) override;

  void updateState();
};

#endif // !CAMERA_MVP_HPP_
