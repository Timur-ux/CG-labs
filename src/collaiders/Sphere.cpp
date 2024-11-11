#include "collaiders/Sphere.hpp"

using namespace collaider;

Sphere::Sphere(float r, Transform &host)
    : CollaiderBase(CollaiderType::Sphere), r_(r), center_(host.position()),
      host_(&host) {}
