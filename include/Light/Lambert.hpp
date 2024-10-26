#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_


#include "Light.hpp"
#include "CameraMVP.hpp"

class LambertLight : public ILight {
  CameraMVP & cameraData_;
  Program & program_;
  glm::vec3 position_;
public:
  LambertLight(glm::vec3 position, CameraMVP & cameraData, Program & program) : position_(position),  cameraData_(cameraData), program_(program) {};

  void setupLightFor(Object & object) override;
};

#endif // !LIGHT_LAMBERT_HPP_
