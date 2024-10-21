#ifndef OBJECT_CUBE_HPP_
#define OBJECT_CUBE_HPP_


#include "Object.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"
#include <GL/gl.h>

class Cube : public Object {
  VertexBuffer data_;
  Texture2D &texture_;

  std::vector<GLfloat> verticiesData_;
  public:
    Cube(GLfloat sideSize, glm::vec3 position, Program & program, Texture2D & texture);

    void draw() override;
};



#endif // !OBJECT_CUBE_HPP_
