#ifndef LIGHT_LAMBERT_HPP_
#define LIGHT_LAMBERT_HPP_

#include "CameraMVP.hpp"
#include "IMoveable.hpp"
#include "Light.hpp"
#include "objects/Object.hpp"

class LambertLight : public ILight {
  CameraMVP &cameraData_;
  Program &program_;
  IMoveable *host_;

  glm::vec4 color_;
  GLfloat kDiffuse_;
  GLfloat kAmbient_;
public:
  LambertLight(IMoveable *host, CameraMVP &cameraData, Program &program,
               glm::vec4 color = {1, 1, 1, 1}, GLfloat kDiffuse = 0.8,
               GLfloat kAmbient = 0.2)
      : host_(host), cameraData_(cameraData), program_(program), color_(color),
        kDiffuse_(kDiffuse), kAmbient_(kAmbient) {};

  void attachTo(IMoveable *newHost) override { host_ = newHost; }

  LightData getData(const Object &object) const override;
};

#endif // !LIGHT_LAMBERT_HPP_
