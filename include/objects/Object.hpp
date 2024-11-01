#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "IMoveable.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Object : public IMoveable {
protected:
  glm::vec3 position_;
  glm::vec3 forward_;
  glm::vec3 up_;
  glm::mat4 model_;

  Program *program_;
  VertexArray vao_;
  VertexBuffer vboData_;
  VertexBuffer vboIndicies_;

  std::vector<Texture *> textures_;

  GLenum drawMode_;
  size_t bufferSize_ = 0;
  size_t indexes_ = 0;
  size_t vertexSize_ = 0;
  size_t textureSize_ = 0;
  size_t normalsSize_ = 0;

  bool moved_ = false;

public:
  Object(glm::vec3 position = glm::vec3(0));
  Object(glm::vec3 position, Program &program,
         std::vector<glm::vec3> verticiesCoords,
         std::vector<glm::vec2> textureCoords, std::vector<glm::vec3> normals,
         std::vector<GLubyte> indexes, GLenum drawMode, Texture2D &texture);

  const glm::vec3 &position() const override { return position_; }
  const glm::mat4 &model() const { return model_; }

  bool setVertexesCoords(const std::vector<glm::vec3> &vertexesCoords);
  bool setTextureCoords(const std::vector<glm::vec2> &textureCoords);
  bool setNormals(const std::vector<glm::vec3> &normals);
  bool setIndexes(const std::vector<GLubyte> &indexes);

  virtual const glm::vec3 &forward() const override { return forward_; };
  virtual const glm::vec3 &up() const override { return up_; };
  virtual bool moved() override {
    if (moved_) {
      moved_ = false;
      return true;
    }
    return false;
  };

  /**
   * @brief Just set:
   *  model matrix as uniform variable
   *  and do basic draw functional
   */
  virtual void draw();

  virtual void draw(Program *otherProgram);
};

#endif // !OBJECT_HPP_
