#include "Light/Lambert.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void LambertLight::setupLightFor(Object & object) {
  GLint nmLoc = glGetUniformLocation(program_.get(), uniforms::directionNormalizeMatrix);
  GLint lpLoc = glGetUniformLocation(program_.get(), uniforms::lightPosition);
  if(nmLoc == -1) {
    std::cerr << "LambertLight: can't find location of " << uniforms::directionNormalizeMatrix << " uniform" << std::endl;
    return;
  }

  if(lpLoc == -1) {
    std::cerr << "LambertLight: can't find location of " << uniforms::lightPosition << " uniform" << std::endl;
    return;
  }

  glCheckError();
  glm::mat4 mv = cameraData_.view() * object.model();
  glCheckError();
  glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mv));
  glCheckError();

  {
    ProgramBind _(program_);
  glCheckError();
    glUniformMatrix3fv(nmLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
  glCheckError();
    glUniform3fv(lpLoc, 1, glm::value_ptr(position_));
  glCheckError();
  }

}


