#ifndef SPHERE_FABRIC_HPP_
#define SPHERE_FABRIC_HPP_

#include "Object.hpp"
#include "Program.hpp"
#include "RigidBody.hpp"
#include "Texture.hpp"
#include "collaiders/AABB.hpp"
#include "collaiders/Sphere.hpp"
#include "fabrics/ObjectFabric.hpp"
#include "meshes/Rectangle.hpp"
#include "meshes/Sphere.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace fabric {
class SphereFabric : public ObjectFabric {
  std::string textureFileName_;
  Texture2D * texture_;
  glm::vec3 pos_;
  float r_;
  glm::vec3 forward_;
  float mass_;
  Program *shaderProgram_ = nullptr;

public:
  SphereFabric(std::string textureFileName, glm::vec3 pos, float r,
            glm::vec3 forward, float mass = 0, Program *shaderProgram = nullptr)
    : pos_(pos), r_(r), forward_(forward), mass_(mass) { 
      if(shaderProgram)
        shaderProgram_ = shaderProgram;
      else
        shaderProgram_ = new Program("./shaders/BlinPhong_shadow.glsl");

      texture_ = new Texture2D(textureFileName_);

    }

  std::shared_ptr<engine::ObjectBase> create() override final {
    mesh_ = new Sphere(r_, pos_, *shaderProgram_, texture_);
    transform_ = mesh_;
    collaider_ = new collaider::Sphere(r_, *transform_);
    if(mass_ > 0) 
      rigidBody_ = new RigidBody(transform_, mass_);
    else
      rigidBody_ = new RigidBody(nullptr, 0);

    return ObjectFabric::create();
  }
};

} // namespace fabric

#endif // !SPHERE_FABRIC_HPP_

