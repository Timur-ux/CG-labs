#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Light/Light.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>


Scene::Scene(Program & program, CameraMVP cameraData, std::vector<ILight *> lights, std::list<Object*> objects) 
    : program_(program), cameraData_(cameraData), objects_(objects), lights_(lights) {}


void Scene::update(double time, double dt) {
  std::stringstream ss;
  ss <<uniforms::lights  << "[0]." << LightData::firstField();
  GLint lightsLoc = program_.getUniformLoc(ss.str().c_str());

  if(lightsLoc == -1)
    throw std::invalid_argument("position of " + std::string(uniforms::lights) + " uniform undefined");

  for(auto & light: lights_) {
    light->renderToShadowMap(objects_);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(Object * object : objects_) {
    for(size_t i = 0; i < lights_.size(); ++i) {
      if(!lights_[i]->getData(*object).setAsUniform(program_, i, lightsLoc, i + 4))
        std::cerr << "Can't set light data as uniform" << std::endl;
      glCheckError();
    }
    object->draw();
    glCheckError();
  }
}
