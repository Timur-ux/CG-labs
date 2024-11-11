#include "CameraMVP.hpp"
#include "IMoveable.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>

CameraMVP::CameraMVP(Program &program, glm::vec3 position, glm::vec3 up,
                     glm::vec3 target, GLfloat fov, GLfloat ratio)
    : Transform(position), program_(program), 
       fov_(fov), ratio_(ratio), view_(1), perspective_(1) {
  lookAt(target);

  viewLoc_ = glGetUniformLocation(program_.get(), "view");
  perspectiveLoc_ = glGetUniformLocation(program_.get(), "perspective");

  view_ = glm::lookAt(position_, target, up_);
  perspective_ = glm::perspective(glm::radians(fov_), ratio_, n, f);

  ProgramBind binding(program_);
  glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view_));
  glUniformMatrix4fv(perspectiveLoc_, 1, GL_FALSE,
                     glm::value_ptr(perspective_));
  glCheckError();
}

void CameraMVP::moveTo(glm::vec3 position) {
  Transform::moveTo(position);
  viewChanged_ = true;
}

void CameraMVP::shiftBy(glm::vec3 shift) {
  Transform::shiftBy(shift);
  viewChanged_ = true;
}

void CameraMVP::updateState() {
  ProgramBind binding(program_);
  if(viewChanged_) {
    view_ = glm::lookAt(position_, position_ + forward_, up_);
    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view_));
    viewChanged_ = false;
  }
  if(perspectiveChanged_) {
    perspective_ = glm::perspective(glm::radians(fov_), ratio_, n, f);
    glUniformMatrix4fv(perspectiveLoc_, 1, GL_FALSE, glm::value_ptr(perspective_));
    perspectiveChanged_ = false;
  }

}

void CameraMVP::lookAt(glm::vec3 target) {
  Transform::lookAt(target);
  viewChanged_ = true;
}

void CameraMVP::lookInto(glm::vec3 direction) {
  Transform::lookInto(direction);
  viewChanged_ = true;
}

void CameraMVP::rotateAround(glm::vec3 v, float rads) {
  Transform::rotateAround(v, rads);
  viewChanged_ = true;
}
void CameraMVP::changeFov(GLfloat newFov) {
  if (newFov <= 0 || newFov > 180)
    throw std::invalid_argument("Fov must be in (0, 180]. Given: " +
                                std::to_string(newFov));

  fov_ = newFov;
}

void CameraMVP::changeRatio(GLfloat newRatio) {
  if (newRatio <= 0)
    throw std::invalid_argument("Ratio must be positive. Given: " +
                                std::to_string(newRatio));

  ratio_ = newRatio;
}
