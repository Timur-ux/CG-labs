#include "objects/Sphere.hpp"
#include "objects/Object.hpp"
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

struct SphereData {
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec3> texCoords;
  std::vector<glm::vec3> normals;
};

/**
 * @brief Generate n^2 points on sphere
 *
 * @param r -- radius
 * @param n -- detalization
 *
 * @return vector of verticies
 */
SphereData generateSphereData(GLfloat r, GLuint n) {
  SphereData result;
  GLfloat yaw0 = 0;
  GLfloat pitch0 = 0;

  GLfloat delta = 360.0f / n;

  for(size_t i = 0; i < n; ++i) {
    for(size_t j = 0; j < n; ++j) {
      GLfloat yaw = glm::radians(yaw0 + delta * i);
      GLfloat pitch = glm::radians(pitch0 + delta * n);

      glm::vec3 direction(
          glm::cos(pitch) * glm::cos(yaw),
          glm::sin(pitch),
          glm::cos(pitch) * glm::sin(yaw)
          );

      result.vertexes.push_back(direction * r);
      result.texCoords.push_back(direction);
      result.normals.push_back(direction);
    }
  }

  return result;
}

Sphere::Sphere(GLfloat r, glm::vec3 position, GLuint detalization)
    : Object(position) {
      // TODO
    }
