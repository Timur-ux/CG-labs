#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <glm/glm.hpp>


namespace geometry {
  /**
   * @brief construct 4x4 transform matrix from current basis to basis
   * created by Gramm-Shmith's ortogonalization and shifted to position
   *
   *
   * @param position -- origin of new basis
   * @param forward -- first orta of new basis(z-axis)
   * @param up -- second vector(non collinear to forward)
   *
   * @return transform matrix
   * x is "right" axis
   * y is "up" axis
   * z is forward
   */
glm::mat4 constructTransform(glm::vec3 position, glm::vec3 forward, glm::vec3 up);

} // !geometry



#endif // !GEOMETRY_HPP_
