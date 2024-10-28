#ifndef OBJECT_CUBE_HPP_
#define OBJECT_CUBE_HPP_


#include "Object.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Rectangle : public Object {
  VertexBuffer dataVBO_;
  Texture2D &texture_;

  public:
    Rectangle(glm::vec3 sideSizes, glm::vec3 position, Program & program, Texture2D & texture);
};



#endif // !OBJECT_CUBE_HPP_
