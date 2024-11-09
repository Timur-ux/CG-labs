#include "Scene.hpp"
#include <vector>

Scene::Scene() : objectUpdateEventHandler_(new UpdateEventHandler), updateEvent(timeUpdateEvent_){}

void Scene::update(const double & time, const double &dt) {
  std::vector<Mesh*> movedObject = objectUpdateEventHandler_->getAndReset();

  timeUpdateEvent_.invoke(time, dt);
  for(auto &[_, camera] : cameras_) {
    camera->recalcObjectsInFrame(movedObject);
    camera->update(time, dt);
  }
}

void Scene::initialUpdate(const double &dt) {
  std::vector<Mesh*> movedObject{};
  for(const auto & it : objects_) movedObject.push_back(it.second);

  timeUpdateEvent_.invoke(0, dt);
  for(auto &[_, camera] : cameras_) {
    camera->recalcObjectsInFrame(movedObject);
    camera->update(0, dt);
  }

}
void Scene::addObject(Mesh* object) {
  objects_[object->getId()] = object;
  object->updateEvent += *objectUpdateEventHandler_;
  updateEvent += *object;
}

void Scene::addCamera(Camera* camera) {
  cameras_[camera->getId()] = camera;
}


void UpdateEventHandler::call(Mesh* object) {
  movedObjects.push_back(object);
}

std::vector<Mesh*> && UpdateEventHandler::getAndReset() {
  return std::move(movedObjects);
}
