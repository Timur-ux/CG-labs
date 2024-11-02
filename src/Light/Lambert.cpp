#include "Light/Lambert.hpp"
#include "Framebuffer.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

LightData LambertLight::getData(const Object &object) {
  // if (host_->moved()) {
  //   glm::mat4 view = glm::lookAt(
  //       host_->position(), host_->position() + host_->forward(), host_->up());
  //   lightSpaceMatrix_ = lightProjection_ * view;
  // }

  return LightData{
      glm::inverseTranspose(glm::mat3(cameraData_.view() * object.model())),
      glm::vec3(host_->position()),
      glm::vec4(color_),
      kDiffuse_,
      kAmbient_,
      0,
      lightSpaceMatrix_,
      depthFramebuffer_
      };
}

void LambertLight::renderToShadowMap(const std::list<Object *> &objects) {
  // if (host_->moved()) {
  //   glm::mat4 view = glm::lookAt(
  //       host_->position(), host_->position() + host_->forward(), host_->up());
  //   lightSpaceMatrix_ = lightProjection_ * view;
  // }

  // temp mark
  glm::mat4 lightProjection, lightView;
  float near_plane = 1.0f, far_plane = 7.5f;
  lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  lightView = glm::lookAt(host_->position(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
  lightSpaceMatrix_ = lightProjection * lightView;

  shadowMapProgram_.bind();
  shadowMapProgram_.setUniformMat4(uniforms::lightSpaceMatrix,
                                   lightSpaceMatrix_);

  // depthFramebuffer_.bind();
  // depthFramebuffer_.bindDepthMap(0);
  glClear(GL_DEPTH_BUFFER_BIT);
  for (auto &object : objects)
    object->draw(&shadowMapProgram_);
  // depthFramebuffer_.unbind();
  shadowMapProgram_.unbind();
}
