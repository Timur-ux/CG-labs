#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "Program.hpp"
#include "events.hpp"
#define GLEW_STATIC
#include "IDrawable.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

using Id = size_t;

template <typename T> using ptr = std::shared_ptr<T>;

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
class Object : public IDrawable,
               public IEventHandler<const double &, const double &> {
  Id id_;

protected:
  GLuint vertexN_;
  Program program_;
  Geometry geometry_;
  GLenum type_ = GL_POINTS;
  Event<Object *> updateEvent_;
  GLfloat *verticies_;
  void (*moveFN_)(GLfloat *, const double & time, const double &dt);
  
  Object(GLuint vertexN, GLuint vertexSize, GLfloat *vertexData,
         Program program, void (*moveFN)(GLfloat *, const double &time, const double &dt) = nullptr)
      : geometry_(vertexN, vertexSize, vertexData), verticies_(vertexData),
        program_(program), vertexN_(vertexN), id_(rand()),
        updateEvent(updateEvent_),
        moveFN_(moveFN){};

public:
  virtual ~Object() {
    delete verticies_;
  };
  IEvent<Object *> &updateEvent;

  /**
   * @brief Draw points on geometry's verticies by default
   */
  virtual void draw() override;
  void setAttrPtr(std::string name, int componentsN, GLsizei stride,
                  void *offset, GLenum type = GL_FLOAT,
                  bool normalized = false);
  Id getId();
  void updateVerticies(GLvoid *verticiesData) {
    geometry_.updateVerticies((GLfloat *)verticiesData);
  };

  virtual void call(const double & time, const double & dt) override {
    if(moveFN_) {
      moveFN_(verticies_,time,  dt);
      updateVerticies(verticies_);
      updateEvent_.invoke(this);// Раз обновили позицию, тыкнем всех подписчеков объекта
    }
  };
};

#endif // !OBJECT_HPP_
