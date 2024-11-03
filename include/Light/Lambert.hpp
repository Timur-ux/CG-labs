#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_

#include "CameraMVP.hpp"
#include "DepthFramebuffer.hpp"
#include "IMoveable.hpp"
#include "Light.hpp"
#include "objects/Object.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class LambertLight : public ILight, public MoveableBase {
  CameraMVP &cameraData_;
  Program &program_;
  Program shadowMapProgram_;

  glm::vec4 color_;
  GLfloat kDiffuse_;
  GLfloat kAmbient_;

  glm::mat4 lightProjection_;
  glm::mat4 lightSpaceMatrix_; //  lightProjection*view

  GLfloat n_ = 1.0f, f_ = 100.0f;
  DepthFramebuffer depthFramebuffer_;

  LightData getData(const Object &object) override;
public:
  LambertLight(glm::vec3 position, glm::vec3 target, CameraMVP &cameraData, Program &program,
               DepthFramebuffer &&framebuffer, glm::vec4 color = {1, 1, 1, 1},
               GLfloat kDiffuse = 0.8, GLfloat kAmbient = 0.2);



  void renderToShadowMap(const std::list<Object *> &objects) override;
  void bindDepthMapTo(int block) override;
  bool setLightParamsFor(const Object & object, size_t i) override;
};

#endif // !LIGHT_LAMBERT_HPP_
