#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "RigidBody.hpp"
#include "collaiders/CollaiderBase.hpp"
#include "IMoveable.hpp"
#include "meshes/Mesh.hpp"
namespace engine {

enum class ComponentType {
  mesh
  , transform 
  , rigidBody
  , collaider
};

class IObject {
  public:
  virtual void *getComponent(ComponentType type) = 0;
  virtual void setComponent(ComponentType type, void * component) = 0;
};

class ObjectBase : public IObject {
  Transform * transform_ = nullptr;
  RigidBody * rigidBody_ = nullptr;
  void * mesh_ = nullptr;
  collaider::CollaiderBase * collaider_ = nullptr;

  public:
  ObjectBase();
  void *getComponent(ComponentType type) override;
  void setComponent(ComponentType type, void * component) override;
};

} //!engine;

#endif // !OBJECT_HPP_
