#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "CameraMVP.hpp"
#include "Light/Light.hpp"
#include "Program.hpp"
#include "meshes/Mesh.hpp"
#include <list>

class Scene {
protected:
  std::vector<Mesh*> objects_;
  CameraMVP cameraData_;
  Program & program_;
  std::vector<ILight *> lights_;

public:
  Scene(Program & program, CameraMVP cameraData, std::vector<ILight *> lights = {}, std::vector<Mesh*> objects = {});

  void update(double time, double dt);
};


#endif // ! SCENE_HPP_
