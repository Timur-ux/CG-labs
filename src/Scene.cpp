#include "Scene.hpp"
#include <glm/gtc/type_ptr.hpp>


Scene::Scene(Program & program, CameraMVP cameraData, std::list<Object*> objects ) 
    : program_(program), cameraData_(cameraData), objects_(objects) {
      updateUniforms();
    }

void Scene::updateUniforms() {
  GLint viewLoc = glGetUniformLocation(program_.get(), "view");
  GLint perspectiveLoc = glGetUniformLocation(program_.get(), "view");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cameraData_.view()));
  glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(cameraData_.perspective()));
}
void Scene::update(double time, double dt) {
  if(cameraData_.isChanged()) {
    updateUniforms();
    cameraData_.setChangedOff();
  }

  for(Object * object : objects_) {
    object->draw();
  }
}
