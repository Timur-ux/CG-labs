#include "VertexArray.hpp"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &vao_);
}

VertexArray::VertexArray(VertexArray && other) {
  vao_ = other.vao_;
  other.vao_ = 0;
}

VertexArray & VertexArray::operator=(VertexArray && other) {
  vao_ = other.vao_;
  other.vao_ = 0;

  return *this;
}
VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &vao_);
}


void VertexArray::bind() {
  glBindVertexArray(vao_);
}

void VertexArray::unbind() {
  glBindVertexArray(vao_);
}
