#include "Light/Light.hpp"
#include "Program.hpp"

bool LightData::setAsUniform(Program & program, size_t i, size_t startLoc) {
  
  return program.setUniformMat3(startLoc + i*8 + 0, normalMatrix)
         && program.setUniformVec3(startLoc + i*8 + 1, lightPosition)
         && program.setUniformVec4(startLoc + i*8 + 2, color)
         && program.setUniformFloat(startLoc + i*8 + 3, kDiffuse)
         && program.setUniformFloat(startLoc + i*8 + 4, kAmbient)
         && program.setUniformFloat(startLoc + i*8 + 5, kGlare)
         && program.setUniformMat4(startLoc + i*8 + 6, lightSpaceMatrix)
         && depthFramebuffer.setDepthTexAsUniform(program, startLoc + i*8 + 7);
}
