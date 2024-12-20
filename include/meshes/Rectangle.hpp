#ifndef OBJECT_CUBE_HPP_
#define OBJECT_CUBE_HPP_


#include "meshes/Mesh.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Rectangle : public Mesh {
  VertexBuffer dataVBO_;
  Texture2D &texture_;

  public:
    Rectangle(glm::vec3 sideSizes, glm::vec3 position, Program & program, Texture2D & texture);
};



#endif // !OBJECT_CUBE_HPP_
