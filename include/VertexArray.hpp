#ifndef VERTEX_ARRAY_HPP_
#define VERTEX_ARRAY_HPP_

#define GLEW_STATIC
#include <GL/glew.h>

class VertexArray {
private:
  GLuint vao_;

public:
  VertexArray(/* data */);

  VertexArray(VertexArray &&other);
  VertexArray &operator=(VertexArray &&other);

  VertexArray(VertexArray &other) = delete;
  VertexArray &operator=(VertexArray &other) = delete;

  ~VertexArray();

  void bind();
  void unbind();
};

struct VAOBind {
  VertexArray &vao;
  GLint oldArray = 0;
  VAOBind(VertexArray &target) : vao(target) {
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &oldArray);
    vao.bind();
  }

  ~VAOBind() {
    vao.unbind();
    glBindVertexArray(oldArray);
  }
};

#endif // !VERTEX_ARRAY_HPP_
