#include "objects/Object.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "glslDataNaming.hpp"
#include <GL/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(glm::vec3 position, Program &program,
               std::vector<glm::vec3> verticiesCoords,
               std::vector<glm::vec2> textureCoords,
               std::vector<glm::vec3> normals
               , std::vector<GLbyte> indexes
               , GLenum drawMode, Texture2D &texture)
    : position_(position), program_(program), model_(1),
      verticiesCoords_(verticiesCoords), textureCoords_(textureCoords),
      normals_(normals), indexes_(indexes), drawMode_(drawMode), texture_(texture) {
  ProgramBind progBinding(program_);
  VAOBind vaoBingind(vao_);

  model_ = glm::translate(model_, position_);
  size_t bufferSize = verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                      textureCoords.size() * sizeof(textureCoords[0]) +
                      normals.size() * sizeof(normals[0]);
  char *buffer = new char[bufferSize]{0};
  vboData_.create(GL_ARRAY_BUFFER, bufferSize, buffer, GL_STATIC_DRAW);
  vboData_.bind();
  vboData_.setSubData(0, verticiesCoords.size() * sizeof(verticiesCoords[0]),
                  verticiesCoords.data());
  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]),
                  textureCoords.size() * sizeof(textureCoords[0]),
                  textureCoords.data());
  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                      textureCoords.size() * sizeof(textureCoords[0]),
                  normals.size() * sizeof(normals[0]), normals.data());
  vboData_.unbind();

  vboIndicies_.create(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(indexes[0]), indexes.data(), GL_STATIC_READ);
}

void Object::draw() {
  ProgramBind programBinding(program_);
  VAOBind vaoBinding(vao_);

  GLuint modelLoc = glGetUniformLocation(program_.get(), uniforms::modelMatrix);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
}
