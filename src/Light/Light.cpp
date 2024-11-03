#include "Light/Light.hpp"
#include "Program.hpp"
#include "glslDataNaming.hpp"
#include <stdexcept>

std::string LightData::firstField() {
  return uniforms::lights + std::string("[0].normalMatrix");
}

bool LightData::setAsUniform(Program &program, size_t i, size_t startLoc) {
  if (i + DEPTH_MAP_OFFSET >= MAXIMUM_TEXTURE_BLOCKS)
    throw std::runtime_error("Texture block overflow. Have no enogh texture "
                             "block to add this depth map");

  return program.setUniformMat3(startLoc + i * 8 + 0, normalMatrix) &&
         program.setUniformVec3(startLoc + i * 8 + 1, lightPosition) &&
         program.setUniformVec4(startLoc + i * 8 + 2, color) &&
         program.setUniformFloat(startLoc + i * 8 + 3, kDiffuse) &&
         program.setUniformFloat(startLoc + i * 8 + 4, kAmbient) &&
         program.setUniformFloat(startLoc + i * 8 + 5, kGlare) &&
         program.setUniformMat4(startLoc + i * 8 + 6, lightSpaceMatrix) &&
         depthFramebuffer.depthMap().setTextureBlock(i + DEPTH_MAP_OFFSET) &&
         program.setUniformInt(startLoc + i * 8 + 7, i + DEPTH_MAP_OFFSET);
}
