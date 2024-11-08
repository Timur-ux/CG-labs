#include "objects/Object.hpp"
#include "IMoveable.hpp"
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

Object::Object() : MoveableBase() {
  up_ = glm::vec3(translateModel_ * glm::vec4(0, 1, 0, 1));
}

Object::Object(glm::vec3 position, Program &program,
               std::vector<glm::vec3> verticiesCoords,
               std::vector<glm::vec2> textureCoords,
               std::vector<glm::vec3> normals, std::vector<GLuint> indexes,
               GLenum drawMode, Texture2D &texture, bool rotate)
    : MoveableBase(position, glm::vec3(0)), program_(&program),
      drawMode_(drawMode), textures_({&texture}) {

  ProgramBind progBinding(*program_);
  VAOBind vaoBingind(vao_);

  glCheckError();
  translateModel_ = glm::translate(translateModel_, position_);
  if (position_ != glm::vec3(0))
    forward_ = glm::normalize(-position_);
  else
    forward_ = glm::vec3(1, 0, 0);

  if (rotate)
    translateModel_ = glm::rotate(translateModel_, glm::radians(45.0f), forward_);
  up_ = glm::vec3(0, 1, 0);
  bufferSize_ = verticiesCoords.size() * sizeof(verticiesCoords[0]) +
                textureCoords.size() * sizeof(textureCoords[0]) +
                normals.size() * sizeof(normals[0]);
  vertexSize_ = verticiesCoords.size() * sizeof(verticiesCoords[0]);
  textureSize_ = textureCoords.size() * sizeof(textureCoords[0]);
  normalsSize_ = normals.size() * sizeof(normals[0]);

  vboData_.create(GL_ARRAY_BUFFER, bufferSize_, nullptr);
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

void Object::setupData(glm::vec3 position, Program &program,
                       std::vector<glm::vec3> verticiesCoords,
                       std::vector<glm::vec2> textureCoords,
                       std::vector<glm::vec3> normals,
                       std::vector<GLuint> indexes, GLenum drawMode,
                       Texture2D *texture) {
  *this = Object(position, program, verticiesCoords, textureCoords, normals,
                 indexes, drawMode, *texture);
  glCheckError();
}

Object::Object(Object &&other) {
  translateModel_ = other.translateModel_;
  position_ = other.position_;
  forward_ = other.forward_;
  up_ = other.up_;

  program_ = other.program_;
  vao_ = std::move(other.vao_);
  vboData_ = std::move(other.vboData_);
  vboIndicies_ = std::move(other.vboIndicies_);

  textures_ = other.textures_;
  drawMode_ = other.drawMode_;

  bufferSize_ = other.bufferSize_;
  indexes_ = other.indexes_;
  vertexSize_ = other.vertexSize_;
  textureSize_ = other.textureSize_;
  normalsSize_ = other.normalsSize_;
  glCheckError();
}

Object & Object::operator=(Object &&other) {
  if(this == &other)
    return *this;

  translateModel_ = other.translateModel_;
  position_ = other.position_;
  forward_ = other.forward_;
  up_ = other.up_;

  program_ = other.program_;
  vao_ = std::move(other.vao_);
  vboData_ = std::move(other.vboData_);
  vboIndicies_ = std::move(other.vboIndicies_);

  textures_ = other.textures_;
  drawMode_ = other.drawMode_;

  bufferSize_ = other.bufferSize_;
  indexes_ = other.indexes_;
  vertexSize_ = other.vertexSize_;
  textureSize_ = other.textureSize_;
  normalsSize_ = other.normalsSize_;
  glCheckError();

  return *this;
}

void Object::draw() {
  glCheckError();
  for (size_t i = 0; i < textures_.size(); ++i) {
    textures_[i]->bind();
  }
  glCheckError();
  ProgramBind _(*program_);
  glCheckError();
  vao_.bind();
  glCheckError();
  vboIndicies_.bind();
  glCheckError();

  if (!program_->setUniformMat4(uniforms::modelMatrix, translateModel_))
    std::cerr << "Can't find location of " << uniforms::modelMatrix
              << " uniform" << std::endl;
  glCheckError();

  glDrawElements(drawMode_, indexes_, GL_UNSIGNED_INT, 0);
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
  glCheckError();
  draw();
  glCheckError();
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
