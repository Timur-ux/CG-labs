#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Object {
protected:
  glm::vec3 position_;
  glm::mat4 model_;

  Program & program_;
  VertexArray vao_;
  VertexBuffer vboData_;
  VertexBuffer vboIndicies_;

  Texture2D & texture_;

  std::vector<glm::vec3> verticiesCoords_;
  std::vector<glm::vec2> textureCoords_;
  std::vector<glm::vec3> normals_;
  std::vector<GLubyte> indexes_;

  GLenum drawMode_;

public:
  Object(glm::vec3 position, Program & program, std::vector<glm::vec3> verticiesCoords, std::vector<glm::vec2> textureCoords, std::vector<glm::vec3> normals, std::vector<GLubyte> indexes, GLenum drawMode, Texture2D &texture);
  const glm::vec3 & position() {return position_;}

  /**
   * @brief Just set:
   *  model matrix as uniform variable
   *  and do basic draw functional
   */
  virtual void draw();
};


#endif // !OBJECT_HPP_
