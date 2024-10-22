#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_


#include "Light.hpp"
#include "CameraMVP.hpp"

class LambertLight : public ILight {
  CameraMVP & cameraData_;
public:
  LambertLight(CameraMVP & cameraData) : cameraData_(cameraData) {};

  void setupLightFor(Object & object) override;
};

#endif // !LIGHT_LAMBERT_HPP_
