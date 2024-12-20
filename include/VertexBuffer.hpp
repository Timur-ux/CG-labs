#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#include <cassert>
#define GLEW_STATIC
#include <GL/glew.h>
class VertexBuffer {
private:
  GLuint vbo_;
  GLenum target_;
  GLboolean isOK_;
public:
  VertexBuffer() : isOK_(false), vbo_(0), target_(0) {}
  VertexBuffer(GLenum target, GLsizeiptr size, void *data,
               GLenum usage = GL_DYNAMIC_DRAW);

  VertexBuffer(VertexBuffer &&other) noexcept;
  VertexBuffer &operator=(VertexBuffer &&other) noexcept;
  VertexBuffer(VertexBuffer &other) = delete;
  VertexBuffer &operator=(VertexBuffer &other) = delete;

  ~VertexBuffer();

  void bind();
  void unbind();
  GLenum type() {return target_;}

  void create(GLenum target, GLsizeiptr size, void *data,
               GLenum usage = GL_DYNAMIC_DRAW);

  GLboolean isOk() { return isOK_; }

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
  GLint oldBuffer = 0;
  VBOBind(VertexBuffer &bufferToBind) : buffer(bufferToBind) {
    assert(("Olny GL_ARRAY_BUFFER permitted", buffer.type() == GL_ARRAY_BUFFER));
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &oldBuffer);
    buffer.bind(); }

  ~VBOBind() { buffer.unbind(); 
    glBindBuffer(GL_ARRAY_BUFFER, oldBuffer);
  }
};

#endif // !VERTEX_BUFFER_HPP_
