#include "objects/Object.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "glCheckError.hpp"
#include "glslDataNaming.hpp"
#include <GL/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

std::ostream &operator<<(std::ostream &os, glm::vec3 &vec) {
  os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
  return os;
}

std::ostream &operator<<(std::ostream &os, glm::vec2 &vec) {
  os << "vec3(" << vec.x << ", " << vec.y << ")" << std::endl;
  return os;
}

Object::Object(glm::vec3 position) {
  ProgramBind progBinding(*program_);
  VAOBind vaoBingind(vao_);

  glCheckError();
  model_ = glm::translate(model_, position_);
  forward_ = glm::vec3(model_ * glm::vec4(0, 0, 1, 1));
  up_ = glm::vec3(model_ * glm::vec4(0, 1, 0, 1));
}

Object::Object(glm::vec3 position, Program &program,
               std::vector<glm::vec3> verticiesCoords,
               std::vector<glm::vec2> textureCoords,
               std::vector<glm::vec3> normals, std::vector<GLubyte> indexes,
               GLenum drawMode, Texture2D &texture, bool rotate)
    : position_(position), program_(&program), model_(1), drawMode_(drawMode),
      textures_({&texture}) {

  ProgramBind progBinding(*program_);
  VAOBind vaoBingind(vao_);

  glCheckError();
  model_ = glm::translate(model_, position_);
  if(position_ != glm::vec3(0))
    forward_ = glm::normalize(position_ - glm::vec3(0));
  else
    forward_ = glm::vec3(1, 0, 0);

  if(rotate)
    model_ = glm::rotate(model_, glm::radians(45.0f), forward_);
  up_ = glm::vec3(glm::vec4(0, 1, 0, 1));
  bufferSize_ = verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                textureCoords.size() * sizeof(textureCoords[0]) +
                normals.size() * sizeof(normals[0]);
  vertexSize_ = verticiesCoords.size() * sizeof(verticiesCoords[0]);
  textureSize_ = textureCoords.size() * sizeof(textureCoords[0]);
  normalsSize_ = normals.size() * sizeof(normals[0]);
  char *buffer = new char[bufferSize_]{0};

  vboData_.create(GL_ARRAY_BUFFER, bufferSize_, buffer);
  delete[] buffer;
  glCheckError();

  vboData_.bind();
  glCheckError();

  vboData_.setSubData(0, verticiesCoords.size() * sizeof(verticiesCoords[0]),
                      verticiesCoords.data());
  GLint loc = program_->getAttribLoc(attributes::vertexPosition);
  if (loc != -1)
    vboData_.setAttribPtr(loc, 3, sizeof(GLfloat) * 3, 0);
  else
    std::cerr << "Can't find location of " << attributes::vertexPosition
              << " attribute" << std::endl;
  glCheckError();

  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]),
                      textureCoords.size() * sizeof(textureCoords[0]),
                      textureCoords.data());
  loc = program_->getAttribLoc(attributes::texturePosition);
  if (loc != -1)
    vboData_.setAttribPtr(
        loc, 2, sizeof(GLfloat) * 2,
        (const void *)(verticiesCoords.size() * sizeof(verticiesCoords[0])));
  else
    std::cerr << "Can't find location of " << attributes::texturePosition
              << " attribute" << std::endl;
  glCheckError();

  vboData_.setSubData(verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                          textureCoords.size() * sizeof(textureCoords[0]),
                      normals.size() * sizeof(normals[0]), normals.data());
  loc = program_->getAttribLoc(attributes::normalToBorder);
  if (loc != -1)
    vboData_.setAttribPtr(
        loc, 3, sizeof(GLfloat) * 3,
        (const void *)(verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                       textureCoords.size() * sizeof(textureCoords[0])));
  else
    std::cerr << "Can't find location of " << attributes::normalToBorder
              << " attribute" << std::endl;
  glCheckError();
  vboData_.unbind();

  vboIndicies_.create(GL_ELEMENT_ARRAY_BUFFER,
                      indexes.size() * sizeof(indexes[0]), indexes.data());

  indexes_ = indexes.size();
  glCheckError();
}

void Object::draw() {
  for (size_t i = 0; i < textures_.size(); ++i) {
    textures_[i]->bind();
  }
  glCheckError();
  // ProgramBind _(*program_);
  vao_.bind();
  vboIndicies_.bind();
  glCheckError();

  if (!program_->setUniformMat4(uniforms::modelMatrix, model_))
    std::cerr << "Can't find location of " << uniforms::modelMatrix
              << " uniform" << std::endl;
  glCheckError();

  glDrawElements(drawMode_, indexes_, GL_UNSIGNED_BYTE, 0);
  glCheckError();

  vboIndicies_.unbind();
  vao_.unbind();
  for (auto &tex : textures_)
    tex->unbind();
  glCheckError();
}

void Object::draw(Program *otherProgram) {
  Program *myProgram = program_;
  program_ = otherProgram;
  draw();
  program_ = myProgram;
}

bool Object::setVertexesCoords(const std::vector<glm::vec3> &vertexesCoords) {
  VBOBind _(vboData_);

  char *buffer;
  size_t bufferSize =
      bufferSize_ - vertexSize_ + vertexesCoords.size() * sizeof(glm::vec3);

  buffer = new char[bufferSize];
  vboData_.getSubData(vertexSize_, bufferSize_ - vertexSize_,
                      buffer + vertexesCoords.size() * sizeof(glm::vec3));

  memcpy(buffer, vertexesCoords.data(),
         vertexesCoords.size() * sizeof(glm::vec3));

  vboData_.setData(bufferSize, buffer);
  bufferSize_ = bufferSize;
  vertexSize_ = vertexesCoords.size() * sizeof(glm::vec3);
  delete[] buffer;

  vboData_.setAttribPtr(program_->getAttribLoc(attributes::vertexPosition), 3,
                        sizeof(glm::vec3), 0);
  return true;
}

bool Object::setTextureCoords(const std::vector<glm::vec2> &textureCoords) {
  VBOBind _(vboData_);

  char *buffer;
  size_t bufferSize =
      bufferSize_ - textureSize_ + textureCoords.size() * sizeof(glm::vec2);

  buffer = new char[bufferSize];
  vboData_.getSubData(0, vertexSize_, buffer);
  vboData_.getSubData(
      vertexSize_ + textureSize_, normalsSize_,
      buffer + (vertexSize_ + textureCoords.size() * sizeof(glm::vec2)));

  memcpy(buffer + vertexSize_, textureCoords.data(),
         textureCoords.size() * sizeof(glm::vec2));

  vboData_.setData(bufferSize, buffer);
  bufferSize_ = bufferSize;
  textureSize_ = textureCoords.size() * sizeof(glm::vec2);

  delete[] buffer;

  vboData_.setAttribPtr(program_->getAttribLoc(attributes::texturePosition), 2,
                        sizeof(glm::vec2), (const void *)vertexSize_);
  return true;
}

bool Object::setNormals(const std::vector<glm::vec3> &normals) {
  VBOBind _(vboData_);

  char *buffer;
  size_t bufferSize =
      bufferSize_ - normalsSize_ + normals.size() * sizeof(glm::vec3);
  buffer = new char[bufferSize];
  vboData_.getSubData(0, vertexSize_ + textureSize_, buffer);
  memcpy(buffer + vertexSize_ + textureSize_, normals.data(),
         normals.size() * sizeof(glm::vec3));

  vboData_.setData(bufferSize, buffer);
  bufferSize_ = bufferSize;
  normalsSize_ = normals.size() * sizeof(glm::vec3);

  delete[] buffer;

  vboData_.setAttribPtr(program_->getAttribLoc(attributes::normalToBorder), 3,
                        sizeof(glm::vec3),
                        (const void *)(vertexSize_ + textureSize_));
  return true;
}

bool Object::setIndexes(const std::vector<GLubyte> &indexes) {
  VBOBind _(vboIndicies_);

  vboIndicies_.setData(indexes.size() * sizeof(GLubyte), indexes.data());
  return true;
}
