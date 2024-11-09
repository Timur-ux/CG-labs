#include <Object.hpp>
#include <cstring>
#include <iostream>
#include <iostream>
#include <stdexcept>


Geometry::Geometry(GLuint vertexN, GLuint vertexSize, GLvoid *vertexData) : vertexN_(vertexN), vertexSize_(vertexSize) {
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertexN * vertexSize, vertexData,
               GL_STREAM_DRAW);
}
void Geometry::updateVerticies(GLfloat * verticiesData) {
  
  bindVBO();
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexN_*vertexSize_, verticiesData);
  unbindVBO();
}

void Geometry::bindVAO() {
  glBindVertexArray(vao_);
}

void Geometry::bindVBO() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void Geometry::unbindVAO() {
  glBindVertexArray(0);
}

void Geometry::unbindVBO() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setAttrPtr(std::string name, int componentsN, GLsizei stride,
                        void *offset, GLenum type, bool normalized) {
  GLint loc = glGetAttribLocation(program_.get(), name.c_str());

  if (loc < 0)
    throw std::runtime_error("Can't find attribute with name: " + name);

  glVertexAttribPointer(loc, componentsN, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
  glEnableVertexAttribArray(loc);
}

void Mesh::draw() {
  program_.bind();
  geometry_.bindVAO();

  glDrawArrays(type_, 0, vertexN_);

  geometry_.unbindVAO();
  program_.unbind();
}

Id Mesh::getId() { return id_; }
