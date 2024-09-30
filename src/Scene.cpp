#include "Scene.hpp"

void Scene::update() {
  std::vector<ptr<Object>> movedObject;
  for(auto & [_, object] : objects_) {
    if(object->isMoved) {
      movedObject.push_back(object);
      object->isMoved = false;
    }
  }

  for(auto &[_, camera] : cameras_) {
    camera->recalcObjectsInFrame(movedObject);
    camera->update();
  }
}

void Scene::addObject(ptr<Object> object) {
  objects_[object->getId()] = object;
}

void Scene::addCamera(ptr<Camera> camera) {
  cameras_[camera->getId()] = camera;
}
