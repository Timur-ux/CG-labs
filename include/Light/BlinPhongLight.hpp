#ifndef BLIN_PHONG_LIGHT_HPP_
#define BLIN_PHONG_LIGHT_HPP_

#include "glslDataNaming.hpp"
#include "Light/Light.hpp"
#include <glm/gtc/type_ptr.hpp>
class BlinPhongLight : public LightBase {
  public:

  BlinPhongLight(glm::vec3 position, glm::vec3 target,
                           CameraMVP &cameraData, Program &blinPhongProgram,
                           DepthFramebuffer &&framebuffer, glm::vec4 color = glm::vec4(1),
                           GLfloat kDiffuse = 1, GLfloat kAmbient = 0.2, GLfloat kGlare = 1)
    : LightBase(position, target, cameraData, blinPhongProgram, std::move(framebuffer), color, kDiffuse, kAmbient, kGlare) {};

  bool setLightParamsFor(const Mesh &object, size_t i) override {
    glm::vec3 cameraPos = cameraData_.position();
    return program_.setUniformVec3(uniforms::cameraPos, cameraPos) 
      && LightBase::setLightParamsFor(object, i);
  }
};

#endif // !BLIN_PHONG_LIGHT_HPP_
