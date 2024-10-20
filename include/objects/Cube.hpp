#ifndef OBJECT_CUBE_HPP_
#define OBJECT_CUBE_HPP_


#include "Object.hpp"
#include "VertexBuffer.hpp"
#include <GL/gl.h>

struct Cube : public Object {
  VertexBuffer verticies_;

  std::vector<GLfloat> verticiesData_;
  public:
    Cube(GLfloat sideSize, glm::vec3 position, glm::vec3 forward, glm::vec3 up, Program & program);

    void draw() override;

 #ifdef DEBUG
    std::vector<GLfloat> & get() {return verticiesData_;}
 #endif
};



#endif // !OBJECT_CUBE_HPP_
