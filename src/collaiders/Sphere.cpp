#include "collaiders/Sphere.hpp"
#include "IMoveable.hpp"

using namespace collaider;

Sphere::Sphere(float r, Transform &host)
    : CollaiderBase(CollaiderType::Sphere, &host), r_(r), center_(host.position()) {
  follow(&host, glm::vec3(0));
}

void Sphere::call() {
  center_ = host_->position();
}
