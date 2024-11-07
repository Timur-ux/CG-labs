#include "Light/Light.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <stdexcept>

std::string LightData::firstField() {
  return uniforms::lights + std::string("[0].normalMatrix");
}

bool LightData::setAsUniform(Program &program, size_t i, size_t startLoc) {
  if (i + DEPTH_MAP_OFFSET >= MAXIMUM_TEXTURE_BLOCKS)
    throw std::runtime_error("Texture block overflow. Have no enogh texture "
                             "block to add this depth map");

  return program.setUniformMat3(startLoc + i * 9 + 0, normalMatrix) &&
         program.setUniformVec3(startLoc + i * 9 + 1, lightPosition) &&
         program.setUniformVec4(startLoc + i * 9 + 2, color) &&
         program.setUniformFloat(startLoc + i * 9 + 3, kDiffuse) &&
         program.setUniformFloat(startLoc + i * 9 + 4, kAmbient) &&
         program.setUniformFloat(startLoc + i * 9 + 5, kGlare) &&
         program.setUniformMat4(startLoc + i * 9 + 6, lightSpaceMatrix)

         && depthFramebuffer.depthMap().setTextureBlock(i + DEPTH_MAP_OFFSET)
         && program.setUniformInt(startLoc + i * 9 + 7, i + DEPTH_MAP_OFFSET)
         && program.setUniformInt(startLoc + i * 9 + 8, enabled);
}

static Program shadowMapProgram("./shaders/depthShader.glsl");
LightBase::LightBase(glm::vec3 position, glm::vec3 target,
                     CameraMVP &cameraData, Program &program,
                     DepthFramebuffer &&framebuffer, glm::vec4 color,
                     GLfloat kDiffuse, GLfloat kAmbient, GLfloat kGlare)
    : MoveableBase(position, target), cameraData_(cameraData),
      program_(program), color_(color), kDiffuse_(kDiffuse),
      kAmbient_(kAmbient), kGlare_(kGlare), shadowMapProgram_(shadowMapProgram),
      depthFramebuffer_(std::move(framebuffer)) {

  lightProjection_ = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, n_, f_);
  glm::mat4 view = glm::lookAt(position_, position_ + forward_, up_);

  lightSpaceMatrix_ = lightProjection_ * view;
};

LightData LightBase::getData(const Object &object) {
  glCheckError();
  LightData res{
      glm::inverseTranspose(glm::mat3(cameraData_.view() * object.model())),
      glm::vec3(position_),
      glm::vec4(color_),
      kDiffuse_,
      kAmbient_,
      kGlare_,
      lightSpaceMatrix_,
      depthFramebuffer_,
      int(enabled_)};
  glCheckError();
  return res;
}

bool LightBase::setLightParamsFor(const Object &object, size_t i) {
  glCheckError();
  GLint lightLoc = program_.getUniformLoc(LightData::firstField().c_str());
  glCheckError();
  if (lightLoc < 0) {
    std::cerr << "Position of " << LightData::firstField() << " is undefined"
              << std::endl;
    return false;
  }
  glCheckError();

  program_.bind();
  glCheckError();
  depthFramebuffer_.depthMap().bind();
  glCheckError();
  bool res = getData(object).setAsUniform(program_, i, lightLoc);
  glCheckError();
  return res;
}

void LightBase::bindDepthMapTo(int block) {
  if (block <= 1 || block > 7)
    throw std::invalid_argument("Block arg must be in [1, 7] range. Given: " +
                                std::to_string(block));

  depthFramebuffer_.depthMap().setTextureBlock(block);
}

void LightBase::renderToShadowMap(const std::vector<Object *> &objects) {
  shadowMapProgram_.bind();
  if (!shadowMapProgram_.setUniformMat4(uniforms::lightSpaceMatrix,
                                        lightSpaceMatrix_))
    std::cerr << "Can't set " << uniforms::lightSpaceMatrix
              << "uniform in depth buffer rendering" << std::endl;

  glCullFace(GL_FRONT);
  depthFramebuffer_.bind();
  glClear(GL_DEPTH_BUFFER_BIT);
  for (auto &object : objects) {
    object->draw(&shadowMapProgram_);
  }
  depthFramebuffer_.unbind();
  shadowMapProgram_.unbind();
  glCullFace(GL_BACK);
  glCheckError();
}
