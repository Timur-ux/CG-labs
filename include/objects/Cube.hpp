#ifndef OBJECT_CUBE_HPP_
#define OBJECT_CUBE_HPP_


#include "Object.hpp"
#include "VertexBuffer.hpp"
#include <GL/gl.h>

class Cube : public Object {
  VertexBuffer verticies_;

  std::vector<GLfloat> verticiesData_;
  public:
    Cube(GLfloat sideSize, glm::vec3 position, Program & program);

    void draw() override;
};



#endif // !OBJECT_CUBE_HPP_
