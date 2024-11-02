#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_

#include "CameraMVP.hpp"
#include "DepthFramebuffer.hpp"
#include "IMoveable.hpp"
#include "Light.hpp"
#include "objects/Object.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class LambertLight : public ILight {
  CameraMVP &cameraData_;
  Program &program_;
  Program shadowMapProgram_;
  IMoveable *host_;

  glm::vec4 color_;
  GLfloat kDiffuse_;
  GLfloat kAmbient_;

  glm::mat4 lightProjection_;
  glm::mat4 lightSpaceMatrix_; //  lightProjection*view

  GLfloat n_ = 1.0f, f_ = 10.0f;
public:
  DepthFramebuffer depthFramebuffer_;
  LambertLight(IMoveable *host, CameraMVP &cameraData, Program &program,
               DepthFramebuffer &&framebuffer, glm::vec4 color = {1, 1, 1, 1},
               GLfloat kDiffuse = 0.8, GLfloat kAmbient = 0.2)

      : host_(host), cameraData_(cameraData), program_(program), color_(color),
        kDiffuse_(kDiffuse), kAmbient_(kAmbient),
        shadowMapProgram_("./shaders/depthShader.glsl"),
        depthFramebuffer_(std::move(framebuffer)) {

    lightProjection_ = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, n_, f_);//glm::perspective(glm::radians(45.0f), 4.0f/3.0f, n_, f_);//
    glm::mat4 view = glm::lookAt(
        host_->position(), glm::vec3(0), glm::vec3(0, 1, 0));
    lightSpaceMatrix_ = lightProjection_ * view;
  };

  void attachTo(IMoveable *newHost) override {
    host_ = newHost;
    glm::mat4 view = glm::lookAt(
        host_->position(), host_->position() + host_->forward(), host_->up());
    lightSpaceMatrix_ = lightProjection_ * view;
  }

  LightData getData(const Object &object) override;
  void renderToShadowMap(const std::list<Object *> &objects) override;
};

#endif // !LIGHT_LAMBERT_HPP_
