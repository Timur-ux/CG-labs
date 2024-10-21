#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Program.hpp"
#include "glCheckError.hpp"


Scene::Scene(Program & program, CameraMVP cameraData, std::list<Object*> objects ) 
    : program_(program), cameraData_(cameraData), objects_(objects) {}


void Scene::update(double time, double dt) {
  for(Object * object : objects_) {
    object->draw();
  }
}
