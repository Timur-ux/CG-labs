#include "Light/Lambert.hpp"
#include "IMoveable.hpp"
#include "Program.hpp"
#include "glslDataNaming.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <string>


LambertLight::LambertLight(glm::vec3 position, glm::vec3 target, CameraMVP &cameraData, Program &program,
               DepthFramebuffer &&framebuffer, glm::vec4 color,
               GLfloat kDiffuse, GLfloat kAmbient)
      : MoveableBase(position, target)
      , cameraData_(cameraData)
      , program_(program)
      , color_(color)
      , kDiffuse_(kDiffuse)
      , kAmbient_(kAmbient)
      , shadowMapProgram_("./shaders/depthShader.glsl")
      , depthFramebuffer_(std::move(framebuffer)) {

    lightProjection_ = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, n_, f_);
    glm::mat4 view = glm::lookAt(
        position_, position_ + forward_, up_);

    lightSpaceMatrix_ = lightProjection_ * view;
  };

LightData LambertLight::getData(const Object &object) {
  return LightData{
      glm::inverseTranspose(glm::mat3(cameraData_.view() * object.model())),
      glm::vec3(position_),
      glm::vec4(color_),
      kDiffuse_,
      kAmbient_,
      0,
      lightSpaceMatrix_,
      depthFramebuffer_
      };
}

bool LambertLight::setLightParamsFor(const Object & object, size_t i) {
  GLint lightLoc = program_.getUniformLoc(LightData::firstField().c_str());
  if(lightLoc < 0) {
    std::cerr << "Position of " << LightData::firstField() << " is undefined" << std::endl;
    return false;
  }

  program_.bind();
  depthFramebuffer_.depthMap().bind();
  return getData(object).setAsUniform(program_, i, lightLoc);
}

void LambertLight::bindDepthMapTo(int block) {
  if(block <= 1 || block > 7) 
    throw std::invalid_argument("Block arg must be in [1, 7] range. Given: " + std::to_string(block));

  depthFramebuffer_.depthMap().setTextureBlock(block);
}

void LambertLight::renderToShadowMap(const std::list<Object *> &objects) {

  shadowMapProgram_.bind();
  if(!shadowMapProgram_.setUniformMat4(uniforms::lightSpaceMatrix,
                                   lightSpaceMatrix_))
    std::cerr << "Can't set " << uniforms::lightSpaceMatrix << "uniform in depth buffer rendering" << std::endl;

  depthFramebuffer_.bind();
  glClear(GL_DEPTH_BUFFER_BIT);
  for (auto &object : objects)
    object->draw(&shadowMapProgram_);
  depthFramebuffer_.unbind();
  shadowMapProgram_.unbind();
}
