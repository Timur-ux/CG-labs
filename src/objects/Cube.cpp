#include "objects/Cube.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "glCheckError.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Counter clock wise from left bottom
static const std::vector<glm::vec3> vertexCoords{
    {-1, -1, -1}, {1, -1, -1},  {1, 1, -1},  {-1, 1, -1}, // front
    {-1, -1, 1},  {1, -1, 1},   {1, 1, 1},   {-1, 1, 1},  // back
    {-1, -1, 1},  {-1, -1, -1}, {-1, 1, -1}, {-1, 1, 1},  // left
    {1, -1, -1},  {1, -1, 1},   {1, 1, 1},   {1, 1, -1},  // right
    {-1, 1, -1},  {1, 1, -1},   {1, 1, 1},   {-1, 1, 1},  // up
    {-1, -1, 1},  {1, -1, 1},   {1, -1, -1}, {-1, -1, -1} // bottom
};

Cube::Cube(GLfloat sideSize, glm::vec3 position, Program &program,
           Texture2D &texture)
    : Object(position, program), texture_(texture) {}

void Cube::draw() {
  Object::draw();
  texture_.bind();
  program_.bind();
  vao_.bind();

  glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_INT, 0);

  vao_.unbind();
  program_.unbind();
  texture_.unbind();
}
