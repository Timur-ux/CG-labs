#include "objects/Rectangle.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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
    {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  // back

    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, // left
    {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  // right

    {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  // up
    {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, // down
};

static const std::vector<GLubyte> indexes{
    2,  1,  0,  0,  3,  2, // front
    5,  6,  7,  7,  4,  5, // back

    10,  9,  8, 8, 11, 10,  // left
    15, 14, 13, 13, 12, 15, // right

    18, 17, 16, 16, 19, 18, // up
    23, 22, 21, 21, 20, 23, // down
};

std::vector<glm::vec2> scaledTextures(float scaleX, float scaleY){
  auto newTextureCoord = textureCoords;
  for(auto & coord : newTextureCoord) {
    coord.x *= scaleX;
    coord.y *= scaleY;
  }

  return newTextureCoord;
}

std::vector<glm::vec2> prepareTextureCoords(glm::vec3 scales, bool scale) {
  if(!scale)
    return textureCoords;
  return scaledTextures(scales.x, scales.z);
}

Rectangle::Rectangle(glm::vec3 sideSize, glm::vec3 position, Program &program,
                     Texture2D &texture, bool rotate)
    : Object(position, program, vertexCoords, prepareTextureCoords(sideSize, true), normals, indexes,
             GL_TRIANGLES, texture, rotate),
      texture_(texture) {
  model_ = glm::scale(model_, sideSize);
}
