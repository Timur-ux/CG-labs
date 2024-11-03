#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Light/Light.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include <iostream>


Scene::Scene(Program & program, CameraMVP cameraData, std::vector<ILight *> lights, std::list<Object*> objects) 
    : program_(program), cameraData_(cameraData), objects_(objects), lights_(lights) {}


void Scene::update(double time, double dt) {
  // render to shadow map
  for(auto & light: lights_) {
    light->renderToShadowMap(objects_);
  }
  glFinish();

  program_.bind();
  // render scene with generated depth map
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(Object * object : objects_) {
    for(size_t i = 0; i < lights_.size(); ++i) {
      if(!lights_[i]->setLightParamsFor(*object, i))
        std::cerr << "Can't set light data as uniform" << std::endl;
    }

    object->draw();
  }

  program_.unbind();
  glCheckError();
}
