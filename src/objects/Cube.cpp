#include "objects/Cube.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(GLfloat sideSize, glm::vec3 position, glm::vec3 forward, glm::vec3 up, Program & program)
    : Object(position, forward, up, program) {
  VAOBind vaoBinding(vao_);

  GLfloat size2 = sideSize / 2;
  std::vector<GLfloat> verticies {
      -size2, -size2, -size2,        size2,  -size2, -size2,
      size2,  size2,  -size2,        -size2, size2,  -size2,

      -size2, -size2, size2, size2, -size2,  size2,
      size2,  size2, size2, -size2,  size2,  size2};
#ifdef DEBUG
  verticies = {
    -1, -1, -1, 1, -1, -1,
    1, 1, -1, -1, 1, -1,
    -1, -1, 1, 1, -1, 1,
    1, 1, 1, -1, 1, 1,
  }
#endif

  VertexBuffer vbo(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat),
                   verticies.data());
  {
    VBOBind vboBinding(vbo);
    GLint index = glGetAttribLocation(program_.get(), "vertexPosition");
    vbo.setAttribPtr(index, 3, 3 * sizeof(GLfloat), 0);
  }

  verticiesData_ = verticies;
  verticies_ = std::move(vbo);
}

void Cube::draw() {
  program_.bind();
  vao_.bind();

  GLuint modelLoc = glGetUniformLocation(program_.get(), "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));

  std::vector<GLuint> indecies {
    0, 1, 2, 2, 3, 0, // front
    4, 5, 6, 6, 7, 4, // back
    0, 3, 4, 4, 7, 3, // left
    1, 2, 6, 6, 5, 1, // right
    2, 3, 7, 7, 6, 2, // up
    0, 1, 5, 5, 4, 0, // down
  };

  glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT, indecies.data());

  vao_.unbind();
  program_.unbind();
}
