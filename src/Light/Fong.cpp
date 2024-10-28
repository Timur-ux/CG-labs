#include "Light/Fong.hpp"
#include "glslDataNaming.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void FongLight::setupLightFor(Object & object) {
  GLint nmLoc = glGetUniformLocation(program_.get(), uniforms::directionNormalizeMatrix);
  GLint lpLoc = glGetUniformLocation(program_.get(), uniforms::lightPosition);
  GLint pLoc = glGetUniformLocation(program_.get(), uniforms::fongP);
  if(nmLoc == -1) {
    std::cerr << "LambertLight: can't find location of " << uniforms::directionNormalizeMatrix << " uniform" << std::endl;
    return;
  }

  if(lpLoc == -1) {
    std::cerr << "LambertLight: can't find location of " << uniforms::lightPosition << " uniform" << std::endl;
    return;
  }

  if(pLoc == -1) {
    std::cerr << "LambertLight: can't find location of " << uniforms::fongP << " uniform" << std::endl;
    return;
  }

  glm::mat4 mv = cameraData_.view() * object.model();
  glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mv));

  {
    ProgramBind _(program_);
    glUniformMatrix3fv(nmLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniform3fv(lpLoc, 1, glm::value_ptr(position_));
    glUniform1f(pLoc, p_);
  }
}
