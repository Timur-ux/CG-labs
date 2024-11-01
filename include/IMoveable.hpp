#ifndef IMOVABLE_HPP_
#define IMOVABLE_HPP_

#include <glm/glm.hpp>

class IMoveable {
public:
  virtual const glm::vec3 &position() const = 0;
  virtual const glm::vec3& forward() const = 0;
  virtual const glm::vec3& up() const = 0;
  virtual bool moved() = 0;
};

#endif // !IMOVABLE_HPP_
