#include "meshes/Pyramid.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include "utils/printGlm.hpp"

static const std::vector<glm::vec3> vertexes{
    {-1, -1, -1}, {1, -1, -1}, {0, 1, 0}, // front
    {-1, -1, 1},  {1, -1, 1},  {0, 1, 0}, // back
    {-1, -1, -1}, {-1, -1, 1}, {0, 1, 0}, // left
    {1, -1, 1},   {1, -1, -1}, {0, 1, 0}, // right

    {-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {-1, -1, 1}, // bottom
};

static const std::vector<glm::vec2> textureCoords{
    {0, 0}, {1, 0}, {0.5, 1}, // front
    {0, 0}, {1, 0}, {0.5, 1}, // back
    {0, 0}, {1, 0}, {0.5, 1}, // left
    {0, 0}, {1, 0}, {0.5, 1}, // right

    {0, 0}, {1, 0}, {1, 1},   {0, 1}, // bottom
};

static const std::vector<GLuint> indexes{
    2,  1,  0,  // front
    3,  4,  5,  // back
    6,  7,  8,  // left
    9,  10, 11, // right

    12, 13, 14, 14, 15, 12, // bottom
};

std::vector<glm::vec3> generateNormals(std::vector<glm::vec3> vertex) {
  std::vector<glm::vec3> normals;
  normals.assign(vertexes.size(), glm::vec3(0));

  glm::vec3 front = -glm::normalize(
      glm::cross(vertexes[2] - vertexes[1], vertexes[0] - vertexes[1]));
  glm::vec3 back = -glm::normalize(
      glm::cross(vertexes[3] - vertexes[4], vertexes[5] - vertexes[4]));
  glm::vec3 left = -glm::normalize(
      glm::cross(vertexes[6] - vertexes[7], vertexes[8] - vertexes[7]));
  glm::vec3 right = -glm::normalize(
      glm::cross(vertexes[9] - vertexes[10], vertexes[11] - vertexes[10]));
  glm::vec3 bottom = glm::vec3(0, -1, 0);

  std::cout << "front: " << front
            << "back: " << back
            << "left: " << left
            << "right: " << right
            ;

  size_t offset = 0;
  for (size_t _ = 0; _ < 3; _++)
    normals[offset++] = front;
  for (size_t _ = 0; _ < 3; _++)
    normals[offset++] = back;
  for (size_t _ = 0; _ < 3; _++)
    normals[offset++] = left;
  for (size_t _ = 0; _ < 3; _++)
    normals[offset++] = right;

  for (size_t _ = 0; _ < 4; _++)
    normals[offset++] = bottom;

  return normals;
}

Pyramid::Pyramid(glm::vec3 position, Program &program, Texture2D *texture,
                 glm::vec3 scale)
    : scale_(scale), Mesh() {
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), scale);
  translateModel_ = scaleMatrix * translateModel_;

  setupData(position, program, vertexes, textureCoords,
            generateNormals(vertexes), indexes, GL_TRIANGLES, texture);
}
