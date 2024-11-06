#ifndef PRITN_GLM_
#define PRITN_GLM_

#include <iostream>
#include <glm/glm.hpp>

inline std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
  os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec) {
  os << "vec3(" << vec.x << ", " << vec.y << ")" << std::endl;
  return os;
}

#endif // !PRITN_GLM_
