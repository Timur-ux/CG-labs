#include "objects/Rectangle.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Counter clock wise from left bottom
static const std::vector<glm::vec3> vertexCoords{
    {-0.5, -0.5, 0}, {0.5, -0.5, 0},  {0.5, 0.5, 0},  {-0.5, 0.5, 0}, // front
    {-0.5, -0.5, 1}, {0.5, -0.5, 1},  {0.5, 0.5, 1},  {-0.5, 0.5, 1}, // back

    {-0.5, -0.5, 1}, {-0.5, -0.5, 0}, {-0.5, 0.5, 0}, {-0.5, 0.5, 1}, // left
    {0.5, -0.5, 0},  {0.5, -0.5, 1},  {0.5, 0.5, 1},  {0.5, 0.5, 0},  // right

    {-0.5, 0.5, 0},  {0.5, 0.5, 0},   {0.5, 0.5, 1},  {-0.5, 0.5, 1},  // up
    {-0.5, -0.5, 1}, {0.5, -0.5, 1},  {0.5, -0.5, 0}, {-0.5, -0.5, 0}, // down
};

// Counter clock wise from left bottom
static const std::vector<glm::vec2> textureCoords{
    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // front
    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // back

    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // left
    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // right

    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // up
    {0, 0}, {1, 0}, {1, 1}, {0, 1}, // down
};

// Counter clock wise from left bottom
static const std::vector<glm::vec3> normals{
    {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, // front
    {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, 1},              // back

    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, // left
    {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  // right

    {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  // up
    {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, // down
};

static const std::vector<GLubyte> indexes{
    0,  1,  2,  3,  255, // front
    4,  5,  6,  7,  255, // back

    8,  9, 10, 11, 255, // left
    12, 13, 14, 15, 255, // right

    16, 17, 18, 19, 255, // up
    20, 21, 22, 23       // down
};

Rectangle::Rectangle(glm::vec3 sideSize, glm::vec3 position, Program &program,
           Texture2D &texture)
    : Object(position, program, vertexCoords, textureCoords, normals, indexes,
             GL_TRIANGLE_FAN, texture),
      texture_(texture) {
  model_ = glm::scale(model_, sideSize);
}
