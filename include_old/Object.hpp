#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "Program.hpp"
#include "events.hpp"
#include <cstring>
#define GLEW_STATIC
#include "IDrawable.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <cstdlib>

using Id = size_t;

template <typename T> using ptr = std::shared_ptr<T>;

using TMoveFN = void (*)(GLfloat *verticies, const double &time, const double &dt);

class Geometry {
  GLuint vao_;
  GLuint vbo_;
  GLuint vertexN_;
  GLuint vertexSize_;

public:
  Geometry(GLuint vertexN, GLuint vertexSize, GLvoid *vertexData);
  void bindVAO();
  void bindVBO();
  void unbindVAO();
  void unbindVBO();

  void updateVerticies(GLfloat *verticiesData);
};
class Mesh : public IDrawable,
               public IEventHandler<const double &, const double &> {
  Id id_;

protected:
  GLuint vertexN_;
  Program program_;
  Geometry geometry_;
  GLenum type_ = GL_POINTS;
  Event<Mesh *> updateEvent_;
  GLfloat *verticies_;
  TMoveFN moveFN_;

  Mesh(GLuint vertexN, GLuint vertexSize, GLfloat *vertexData,
         Program program, TMoveFN moveFN = nullptr)

      : geometry_(vertexN, vertexSize, vertexData), verticies_(vertexData),
        program_(program), vertexN_(vertexN), id_(rand()),
        updateEvent(updateEvent_), moveFN_(moveFN){
          verticies_ = new GLfloat[vertexN*3];
          memcpy(verticies_, vertexData, vertexN*vertexSize);

          program_.bind();
          geometry_.bindVAO();
          geometry_.bindVBO();

          setAttrPtr("pos", 3, sizeof(GLfloat) * 3, 0);

          geometry_.unbindVBO();
          geometry_.unbindVAO();

          program_.unbind();
        };

public:
  virtual ~Mesh() { delete verticies_; };
  IEvent<Mesh *> &updateEvent;

  /**
   * @brief Draw figure on geometry's verticies by default
   */
  virtual void draw() override;
  void setAttrPtr(std::string name, int componentsN, GLsizei stride,
                  void *offset, GLenum type = GL_FLOAT,
                  bool normalized = false);
  Id getId();
  void updateVerticies(GLvoid *verticiesData) {
    geometry_.updateVerticies((GLfloat *)verticiesData);
  };

  virtual void call(const double &time, const double &dt) override {
    if (moveFN_) {
      moveFN_(verticies_, time, dt);
      updateVerticies(verticies_);
      updateEvent_.invoke(this); // Раз обновили позицию, тыкнем всех подписчеков объекта
    }
  };
};

#endif // !OBJECT_HPP_
