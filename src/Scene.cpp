#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Light/Light.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include <iostream>


Scene::Scene(Program & program, CameraMVP cameraData, std::vector<ILight *> lights, std::vector<Object*> objects) 
    : program_(program), cameraData_(cameraData), objects_(objects), lights_(lights) {}


void Scene::update(double time, double dt) {
  // render to shadow map
  glCheckError();
  for(auto & light: lights_) {
    if(light)
      light->renderToShadowMap(objects_);
  }
  glCheckError();
  glFinish();

  program_.bind();
  // render scene with generated depth map
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(Object * object : objects_) {
    for(size_t i = 0; i < lights_.size(); ++i) {
      if(!lights_[i])
        continue;
      if(!lights_[i]->setLightParamsFor(*object, i))
        std::cerr << "Can't set light data as uniform" << std::endl;
    }

  glCheckError();
    object->draw();
  glCheckError();
  }

  program_.unbind();
  glCheckError();
}
