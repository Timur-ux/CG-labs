#ifndef OBJECT_FABRIC_HPP_
#define OBJECT_FABRIC_HPP_

#include "IMoveable.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "Texture.hpp"
#include "collaiders/CollaiderBase.hpp"
#include "meshes/Mesh.hpp"
#include <memory>

namespace fabric {

class IObjectFabric {
public:
  virtual std::shared_ptr<engine::ObjectBase> create() = 0;
};

class ObjectFabric : public IObjectFabric {
protected:
  Mesh * mesh_ = nullptr;
  collaider::CollaiderBase * collaider_ = nullptr;
  RigidBody * rigidBody_ = nullptr;
  Transform * transform_ = nullptr;

  glm::vec3 size = glm::vec3(0);
public:
  static std::shared_ptr<engine::ObjectBase> create(IObjectFabric &fabric) {
    return fabric.create();
  }

  virtual std::shared_ptr<engine::ObjectBase> create() override {
    auto object = std::make_shared<engine::ObjectBase>();
    if(mesh_)
      object->setComponent(engine::ComponentType::mesh, mesh_);
    if(rigidBody_)
      object->setComponent(engine::ComponentType::rigidBody, rigidBody_);
    if(collaider_)
      object->setComponent(engine::ComponentType::collaider, collaider_);
    if(transform_)
      object->setComponent(engine::ComponentType::transform, transform_);

    return object;
  }
};

} // namespace fabric

#endif // OBJECT_FABRIC_HPP_
