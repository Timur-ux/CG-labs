#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "IMoveable.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>

class Mesh : public MoveableBase {
protected:
  Program *    program_;
  VertexArray  vao_;
  VertexBuffer vboData_;
  VertexBuffer vboIndicies_;

  std::vector<Texture *> textures_;

  GLenum drawMode_;
  size_t bufferSize_ = 0;
  size_t indexes_    = 0;
  size_t vertexSize_ = 0;
  size_t textureSize_ = 0;
  size_t normalsSize_ = 0;

public:
  Mesh();
  Mesh(glm::vec3 position, Program &program,
         std::vector<glm::vec3> verticiesCoords,
         std::vector<glm::vec2> textureCoords, std::vector<glm::vec3> normals,
         std::vector<GLuint> indexes, GLenum drawMode, Texture2D &texture, bool rotate = false);

  Mesh(Mesh &) = delete;
  Mesh(Mesh &&);

  Mesh & operator=(Mesh&) = delete;
  Mesh & operator=(Mesh&&);

  void setupData(glm::vec3 position, Program &program,
         std::vector<glm::vec3> verticiesCoords,
         std::vector<glm::vec2> textureCoords, std::vector<glm::vec3> normals,
         std::vector<GLuint> indexes, GLenum drawMode, Texture2D *texture);
  bool setVertexesCoords(const std::vector<glm::vec3> &vertexesCoords);
  bool setTextureCoords(const std::vector<glm::vec2> &textureCoords);
  bool setNormals(const std::vector<glm::vec3> &normals);
  bool setIndexes(const std::vector<GLubyte> &indexes);


  /**
   * @brief Just set:
   *  model matrix as uniform variable
   *  and do basic draw functional
   */
  virtual void draw();

  virtual void draw(Program *otherProgram);
};

#endif // !OBJECT_HPP_
