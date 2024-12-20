#ifndef COLLAIDER_BASE_HPP_
#define COLLAIDER_BASE_HPP_

#include "IMoveable.hpp"
namespace collaider {
enum class CollaiderType {
  AABB
  , Sphere
};

class CollaiderBase : public Transform {
  CollaiderType type_;
protected:
  Transform *host_;
  CollaiderBase(CollaiderType type, Transform * host) : Transform(host->position(), host->forward()),type_(type), host_(host) { }
public:
  CollaiderType type() {return type_;}
  Transform * host()  {return host_;}
};
}
#endif // !COLLAIDER_BASE_HPP_
