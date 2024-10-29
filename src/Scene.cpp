#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Light/Light.hpp"
#include "Program.hpp"
#include "glCheckError.hpp"
#include <iostream>


Scene::Scene(Program & program, CameraMVP cameraData, ILight &light , std::list<Object*> objects) 
    : program_(program), cameraData_(cameraData), objects_(objects), light_(light) {}


void Scene::update(double time, double dt) {
  for(Object * object : objects_) {
    light_.setupLightFor(*object);
    glCheckError();
    object->draw();
    glCheckError();
  }
  glCheckError();
}
