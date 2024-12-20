#ifndef BOX_FABRIC_HPP_
#define BOX_FABRIC_HPP_

#include "Object.hpp"
#include "Program.hpp"
#include "RigidBody.hpp"
#include "Texture.hpp"
#include "collaiders/AABB.hpp"
#include "fabrics/ObjectFabric.hpp"
#include "meshes/Rectangle.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace fabric {
class BoxFabric : public ObjectFabric {
  Texture2D * texture_;
  glm::vec3 forward_;
  Program *shaderProgram_ = nullptr;

public:
  float mass;
  glm::vec3 pos;
  glm::vec3 size;

  BoxFabric(std::string textureFileName, glm::vec3 pos, glm::vec3 size,
            glm::vec3 forward, float mass, Program *shaderProgram)
    : pos(pos), size(size), forward_(forward), mass(mass) { 
      shaderProgram_ = shaderProgram;

      texture_ = new Texture2D(textureFileName);

    }

  std::shared_ptr<engine::ObjectBase> create() override final {
    mesh_ = new Rectangle(size, pos, *shaderProgram_, *texture_);
    transform_ = mesh_;
    collaider_ = new collaider::AxisAlignedBB(size.x, size.y, size.z, transform_);
    if(mass > 0) 
      rigidBody_ = new RigidBody(transform_, mass);
    else
      rigidBody_ = new RigidBody(nullptr, 0);

    return ObjectFabric::create();
  }

  void setTexture(std::string newTextureFileName) {
      texture_ = new Texture2D(newTextureFileName);
  }

};

} // namespace fabric

#endif // !BOX_FABRIC_HPP_
