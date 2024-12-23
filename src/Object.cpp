#include "Object.hpp"
#include "meshes/Mesh.hpp"
#include <glm/matrix.hpp>

using namespace engine;

void ObjectBase::setComponent(ComponentType type, void * component) {
  switch (type) {
    case ComponentType::mesh:
      mesh_ = (Mesh*)component;
      break;
    case ComponentType::transform:
      transform_ = (Transform*)component;
      break;
    case ComponentType::rigidBody:
      rigidBody_ = (RigidBody*)component;
      break;
    case ComponentType::collaider:
      collaider_ = (collaider::CollaiderBase*)component;
      break;
  }
}

void * ObjectBase::getComponent(ComponentType type) {
  switch (type) {
    case ComponentType::mesh:
      return mesh_;
    case ComponentType::transform:
      return transform_;
    case ComponentType::rigidBody:
      return rigidBody_;
    case ComponentType::collaider:
      return collaider_;
  }
}

ObjectBase::~ObjectBase() {
  if(collaider_)
    delete collaider_;
  if(rigidBody_)
    delete rigidBody_;
  if(mesh_)
    delete mesh_;
}
