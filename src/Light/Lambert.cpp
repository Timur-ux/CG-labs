#include "Light/Lambert.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


LightData LambertLight::getData(const Object & object) const {
  return LightData{
    glm::inverseTranspose(glm::mat3(cameraData_.view() * object.model())),
    glm::vec3(host_->position()),
    glm::vec4(color_),
    kDiffuse_,
    kAmbient_,
    0
  };
}
