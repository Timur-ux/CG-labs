#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#define GLEW_STATIC
#include <GL/glew.h>

class VertexBuffer {
private:
  GLuint vbo_;
  GLenum target_;

public:
  VertexBuffer(GLenum target, GLsizeiptr size, void *data,
               GLenum usage = GL_DYNAMIC_DRAW);

  VertexBuffer(VertexBuffer &&other) noexcept;
  VertexBuffer &operator=(VertexBuffer &&other) noexcept;
  VertexBuffer(VertexBuffer &other) = delete;
  VertexBuffer &operator=(VertexBuffer &other) = delete;

  ~VertexBuffer();

  void bind();
  void unbind();

  void setAttribPtr(GLuint idx, GLint componentsN, GLsizei stride,
                    const void *offset, GLenum type = GL_FLOAT,
                    GLboolean normalized = GL_FALSE);

  void setData(GLsizei size, const void *data, GLenum usage = GL_DYNAMIC_DRAW);

  void resetData();

  void setSubData(GLintptr offset, GLsizeiptr size, const void * data);
  void getSubData(GLintptr offset, GLsizeiptr size, void * ptr);
};

struct VBOBind {
  VertexBuffer &buffer;
  VBOBind(VertexBuffer &bufferToBind) : buffer(bufferToBind) { buffer.bind(); }

  ~VBOBind() { buffer.unbind(); }
};

#endif // !VERTEX_BUFFER_HPP_
