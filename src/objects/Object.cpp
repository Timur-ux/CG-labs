#include "objects/Object.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"
#include <GL/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(glm::vec3 position, Program &program,
               std::vector<glm::vec3> verticiesCoords,
               std::vector<glm::vec2> textureCoords,
               std::vector<glm::vec3> normals, std::vector<GLubyte> indexes,
               GLenum drawMode, Texture2D &texture)
    : position_(position), program_(program), model_(1),
      verticiesCoords_(verticiesCoords), textureCoords_(textureCoords),
      normals_(normals), indexes_(indexes), drawMode_(drawMode),
      texture_(texture) {
  ProgramBind progBinding(program_);
  VAOBind vaoBingind(vao_);

  glCheckError();
  model_ = glm::translate(model_, position_);
  size_t bufferSize = verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                      textureCoords.size() * sizeof(textureCoords[0]) +
                      normals.size() * sizeof(normals[0]);
  char *buffer = new char[bufferSize]{0};

  vboData_.create(GL_ARRAY_BUFFER, bufferSize, buffer);
  delete[] buffer;
  glCheckError();

  vboData_.bind();
  glCheckError();

  vboData_.setSubData(0, verticiesCoords.size() * sizeof(verticiesCoords[0]),
                      verticiesCoords.data());
  GLint loc = glGetAttribLocation(program_.get(), attributes::vertexPosition);
  if(loc != -1) 
    vboData_.setAttribPtr(loc, 3, sizeof(GLfloat) * 3, 0);
  else
    std::cerr << "Can't find location of " << attributes::vertexPosition << " attribute" << std::endl;
  glCheckError();

  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]),
                      textureCoords.size() * sizeof(textureCoords[0]),
                      textureCoords.data());
  loc = glGetAttribLocation(program_.get(), attributes::texturePosition);
  if(loc != -1)
    vboData_.setAttribPtr(
        loc, 2, sizeof(GLfloat) * 2,
        (const void *)(verticiesCoords.size() * sizeof(verticiesCoords[0])));
  else
    std::cerr << "Can't find location of " << attributes::texturePosition << " attribute" << std::endl;
  glCheckError();

  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                          textureCoords.size() * sizeof(textureCoords[0]),
                      normals.size() * sizeof(normals[0]), normals.data());
  loc = glGetAttribLocation(program_.get(), attributes::normalToBorder);
  if(loc != -1)
    vboData_.setAttribPtr(
        loc, 3, sizeof(GLfloat) * 3,
        (const void *)(verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                       textureCoords.size() * sizeof(textureCoords[0])));
  else
    std::cerr << "Can't find location of " << attributes::normalToBorder << " attribute" << std::endl;
  glCheckError();
  vboData_.unbind();

  vboIndicies_.create(GL_ELEMENT_ARRAY_BUFFER,
                      indexes.size() * sizeof(indexes[0]), indexes.data());
  glCheckError();
}

void Object::draw() {
  texture_.bind();
  program_.bind();
  vao_.bind();
  vboIndicies_.bind();

  glCheckError();
  GLint modelLoc = glGetUniformLocation(program_.get(), uniforms::modelMatrix);
  if(modelLoc != -1)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
  else
    std::cerr << "Can't find location of " << uniforms::modelMatrix << " uniform" << std::endl;
  glCheckError();

  glDrawElements(drawMode_, indexes_.size(), GL_UNSIGNED_BYTE, 0);
  glCheckError();

  vboIndicies_.unbind();
  vao_.unbind();
  program_.unbind();
  texture_.unbind();
  glCheckError();
}
