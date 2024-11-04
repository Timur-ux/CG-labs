#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_

#include "Light.hpp"
class LambertLight : public LightBase {
public:
  LambertLight(glm::vec3 position, glm::vec3 target, CameraMVP &cameraData, Program &lambertProgram,
               DepthFramebuffer &&framebuffer, glm::vec4 color = {1, 1, 1, 1},
               GLfloat kDiffuse = 0.8, GLfloat kAmbient = 0.2)
    : LightBase(position, target, cameraData, lambertProgram, std::move(framebuffer), color, kDiffuse, kAmbient, 0) {};
};

#endif // !LIGHT_LAMBERT_HPP_
