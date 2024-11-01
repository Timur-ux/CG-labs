#include "Light/Lambert.hpp"
#include "Framebuffer.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

LightData LambertLight::getData(const Object &object) {
  if (host_->moved()) {
    glm::mat4 view = glm::lookAt(
        host_->position(), host_->position() + host_->forward(), host_->up());
    lightSpaceMatrix_ = lightProjection_ * view;
  }

  return LightData{
      glm::inverseTranspose(glm::mat3(cameraData_.view() * object.model())),
      glm::vec3(host_->position()),
      glm::vec4(color_),
      kDiffuse_,
      kAmbient_,
      0,
      lightSpaceMatrix_,
      shadowMapFramebuffer_.depthBuffer()};
}

void LambertLight::renderToShadowMap(const std::list<Object *> &objects) {
  if (host_->moved()) {
    glm::mat4 view = glm::lookAt(
        host_->position(), host_->position() + host_->forward(), host_->up());
    lightSpaceMatrix_ = lightProjection_ * view;
  }

  ProgramBind programBind(shadowMapProgram_);
  shadowMapFramebuffer_.bind();
  shadowMapProgram_.setUniformMat4(uniforms::lightSpaceMatrix,
                                   lightSpaceMatrix_);


  glClear(GL_DEPTH_BUFFER_BIT);
  for (auto &object : objects)
    object->draw(&shadowMapProgram_);
  shadowMapFramebuffer_.unbind();
}
