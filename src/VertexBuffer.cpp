#include "VertexBuffer.hpp"
#include "glCheckError.hpp"
#include <iostream>

VertexBuffer::VertexBuffer(GLenum target, GLsizeiptr size, void *data,
                           GLenum usage)
    : target_(target) {
  glGetError();

  glGenBuffers(1, &vbo_);
  glBindBuffer(target, vbo_);
  glBufferData(target, size, data, usage);

  GLenum errorCode = glGetError();
  if (errorCode != GL_NO_ERROR)
    std::cerr << "Error while creating Vertex Buffer: " << errorCode
              << std::endl;
  isOK_ = true;
  glBindBuffer(target, 0);
}

void VertexBuffer::create(GLenum target, GLsizeiptr size, void *data, GLenum usage) {
  target_ = target;
  glGetError();

  glGenBuffers(1, &vbo_);
  glBindBuffer(target, vbo_);
  glBufferData(target, size, data, usage);
  glBindBuffer(target, 0);

  GLenum errorCode = glGetError();
  if (errorCode != GL_NO_ERROR)
    std::cerr << "Error while creating Vertex Buffer: " << errorCode
              << std::endl;
  isOK_ = true;
}

VertexBuffer::~VertexBuffer() { 
  if ( vbo_ )
    glDeleteBuffers(1, &vbo_); 
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept {
  vbo_ = other.vbo_;
  other.vbo_ = 0;
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if(this == &other)
    return *this;
  vbo_ = other.vbo_;
  other.vbo_ = 0;

  return *this;
}

void VertexBuffer::bind() {if ( vbo_ ) glBindBuffer(target_, vbo_); }

void VertexBuffer::unbind() { if ( vbo_ ) glBindBuffer(target_, 0); }

void VertexBuffer::setAttribPtr(GLuint idx, GLint componentsN, GLsizei stride,
                                const void *offset, GLenum type,
                                GLboolean normalized) {
  if (!isOK_) return;
  glCheckError();

  glVertexAttribPointer(idx, componentsN, type, normalized, stride, offset);
  glCheckError();
  glEnableVertexAttribArray(idx);
  glCheckError();
}

void VertexBuffer::setData(GLsizei size, const void *data, GLenum usage) {
  bind();

  if(isOK_)
    glBufferData(target_, size, data, usage);
}

void VertexBuffer::resetData() {
  bind();

  if(isOK_)
    glBufferData(target_, 1, nullptr, GL_DYNAMIC_DRAW);
}


void VertexBuffer::setSubData(GLintptr offset, GLsizeiptr size, const void * data) {
  bind();

  if(isOK_)
    glBufferSubData(target_, offset, size, data);
}


void VertexBuffer::getSubData(GLintptr offset, GLsizeiptr size, void * ptr) {
  bind();

  if(isOK_)
    glGetBufferSubData(target_, offset, size, ptr);
}
