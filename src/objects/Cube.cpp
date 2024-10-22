#include "objects/Cube.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "glCheckError.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Cube::Cube(GLfloat sideSize, glm::vec3 position, Program &program,
           Texture2D &texture)
    : Object(position, program), texture_(texture) {
  VAOBind vaoBinding(vao_);

  GLfloat size2 = sideSize / 2;
  GLfloat verticiesCoords[48]{
      -size2, -size2, -size2, // 0th
      size2, -size2, -size2,  // 1th
      size2, size2, -size2,   // 2th
      -size2, size2, -size2,  // 3th
      -size2, -size2, size2,  // 4th
      size2, -size2, size2,   // 5th
      size2, size2, size2,    // 6th
      -size2, size2, size2,   // 7th

      // Old verticies with new texel coords for left and right
      -size2, -size2, -size2, // 0th
      size2, -size2, -size2,  // 1th
      size2, size2, -size2,   // 2th
      -size2, size2, -size2,  // 3th

      // Old verticies with new texel coords for up and down
      size2, size2, -size2,   // 2th
      -size2, size2, -size2,  // 3th
      -size2, -size2, -size2, // 0th
      size2, -size2, -size2,  // 1th
  };

  GLfloat textureCoords[32]{
      0.0f, 0.0f, // 0th
      1.0f, 0.0f, // 1th
      1.0f, 1.0f, // 2th
      0.0f, 1.0f, // 3th
      0.0f, 0.0f, // 4th
      1.0f, 0.0f, // 5th
      1.0f, 1.0f, // 6th
      0.0f, 1.0f, // 7th

      1.0f, 0.0f, // 0th
      0.0f, 0.0f, // 1th
      0.0f, 1.0f, // 2th
      1.0f, 1.0f, // 3th

      1.0f, 0.0f, // 2th
      0.0f, 0.0f, // 3th
      0.0f, 1.0f, // 0th
      1.0f, 1.0f, // 1th
  };

  size_t bufferSize = sizeof(verticiesCoords) + sizeof(textureCoords);
  char *buffer = new char[bufferSize];

  VertexBuffer dataVBO(GL_ARRAY_BUFFER, bufferSize, buffer, GL_STATIC_DRAW);
  delete[] buffer;

  glCheckError();
  {
    VBOBind binding(dataVBO);
    glCheckError();
    dataVBO.setSubData(0, sizeof(verticiesCoords), verticiesCoords);

    GLint index = glGetAttribLocation(program_.get(), "vertexPosition");
    glCheckError();
    dataVBO.setAttribPtr(index, 3, 3 * sizeof(GLfloat), 0);
    glCheckError();

    dataVBO.setSubData(sizeof(verticiesCoords), sizeof(textureCoords),
                       textureCoords);

    index = glGetAttribLocation(program_.get(), "textureCoord");
    dataVBO.setAttribPtr(index, 2, 2 * sizeof(GLfloat),
                         (const void *)sizeof(verticiesCoords));
  }

  dataVBO_ = std::move(dataVBO);
}

void Cube::draw() {
  texture_.bind();
  program_.bind();
  vao_.bind();

  GLuint modelLoc = glGetUniformLocation(program_.get(), "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));

  std::vector<GLuint> indecies{
      0,  1,  2, 2, 3,  0,  // front
      4,  5,  6, 6, 7,  4,  // back
      8,  4,  7, 7, 11, 8,  // left
      9,  10, 6, 6, 5,  9,  // right
      12, 13, 7, 7, 6,  12, // up
      14, 15, 5, 5, 4,  14, // down
  };

  glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT,
                 indecies.data());

  vao_.unbind();
  program_.unbind();
  texture_.unbind();
}
