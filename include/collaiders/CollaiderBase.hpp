#ifndef COLLAIDER_BASE_HPP_
#define COLLAIDER_BASE_HPP_

namespace collaider {
enum class CollaiderType {
  AABB
  , Sphere
};

class CollaiderBase {
  CollaiderType type_;
protected:
  CollaiderBase(CollaiderType type) : type_(type) {}
public:
  CollaiderType type() {return type_;}
};
}
#endif // !COLLAIDER_BASE_HPP_
