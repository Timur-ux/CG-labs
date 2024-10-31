#ifndef IMOVABLE_HPP_
#define IMOVABLE_HPP_

#include <glm/glm.hpp>

class IMoveable {
public:
  virtual const glm::vec3 &position() const = 0;
};

#endif // !IMOVABLE_HPP_
