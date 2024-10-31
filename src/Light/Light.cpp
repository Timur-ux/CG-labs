#include "Light/Light.hpp"
#include "Program.hpp"

bool LightData::setAsUniform(Program & program, size_t i, size_t startLoc) {
  
  ProgramBind _(program);
  return program.setUniformMat3(startLoc + i*6 + 0, normalMatrix)
         && program.setUniformVec3(startLoc + i*6 + 1, lightPosition)
         && program.setUniformVec4(startLoc + i*6 + 2, color)
         && program.setUniformFloat(startLoc + i*6 + 3, kDiffuse)
         && program.setUniformFloat(startLoc + i*6 + 4, kAmbient)
         && program.setUniformFloat(startLoc + i*6 + 5, kGlare);
}
