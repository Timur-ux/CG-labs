#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "CameraMVP.hpp"
#include "Program.hpp"
#include "objects/Object.hpp"
#include <list>

class Scene {
  std::list<Object*> objects_;
  CameraMVP cameraData_;
  Program & program_;

public:
  Scene(Program & program, CameraMVP cameraData, std::list<Object*> objects = {});

  void update(double time, double dt);
};


#endif // ! SCENE_HPP_
