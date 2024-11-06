#include "objects/Sphere.hpp"
#include "Program.hpp"
#include "objects/Object.hpp"
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

struct SphereData {
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec2> texCoords;
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

  GLfloat yawD = 360.0f / n;
  GLfloat pitchD = 180.0f / n * 2;
  GLfloat pitch0 = -90.0f;
  GLfloat du = 1.0f / n * 2;
  GLfloat dv = 1.0f / n;

  size_t iN = n / 2, jN = n;
  for(size_t i = 0; i <= iN; ++i) {
    for(size_t j = 0; j <= jN; ++j) {
      // direction
      GLfloat yaw = glm::radians(yawD * j);
      GLfloat pitch = glm::radians(pitch0 + pitchD * i);

      glm::vec3 direction(
          glm::cos(pitch) * glm::cos(yaw),
          glm::sin(pitch),
          glm::cos(pitch) * glm::sin(yaw)
          );

      result.vertexes.push_back(direction * r);
      result.normals.push_back(direction);
      // uv texture coord
      GLfloat u = i * du, v = j * dv;
      result.texCoords.push_back(glm::vec2(u, v));
    }
  }

  return result;
}

/**
 * @brief Generate indexes vector for drawing mode GL_TRIANGLES
 *
 * @param n -- detalization used in generateSphereData
 *
 * @return 
 */
std::vector<GLuint> generateIndexes(GLuint n) {
  std::vector<GLuint> indexes;

  size_t iN = n / 2, jN = n;
  for(size_t i = 0; i < iN; ++i) {
    for(size_t j = 0; j < jN; ++j) {
      indexes.push_back(i*(jN + 1) + j);
      indexes.push_back((i + 1)*(jN + 1) + j);
      indexes.push_back(i*(jN + 1) + j + 1);

      indexes.push_back((i + 1)*(jN + 1) + j);
      indexes.push_back((i + 1)*(jN + 1) + j + 1);
      indexes.push_back(i*(jN + 1) + j + 1);
    }
  }

  return indexes;
}

Sphere::Sphere(GLfloat r, glm::vec3 position,  Program & program, Texture2D *texture, GLuint detalization)
    : Object() {
      SphereData data = generateSphereData(r, detalization);
      std::vector<GLuint> indexes = generateIndexes(detalization);
      setupData(position, program, data.vertexes, data.texCoords, data.normals, indexes, GL_TRIANGLES, texture);
}
