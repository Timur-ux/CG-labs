#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "IDrawable.hpp"
#include <memory>

using Id = size_t;

template<typename T>
using ptr = std::shared_ptr<T>;

class Geometry {

};

class Object : public IDrawable {
  Id id;
  Geometry geometry;
  public:

  bool isMoved = false;
  virtual void draw(GLFWwindow * win) override;
  Id getId();
};

#endif // !OBJECT_HPP_
