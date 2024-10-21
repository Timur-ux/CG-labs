#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "Program.hpp"
#include "VertexArray.hpp"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Object {
protected:
  glm::vec3 position_;
  glm::vec3 forward_;
  glm::vec3 up_;

  glm::mat4 model_;

  Program & program_;
  VertexArray vao_;

public:
  Object(glm::vec3 position, Program & program);
  const glm::vec3 & position() {return position_;}

  /**
   * @brief Just set model matrix as uniform variable
   * Childs must call verticies draw functions theyself
   */
  virtual void draw();

 #ifdef DEBUG
  glm::mat4 & getModel() {return model_;};
 #endif

};


#endif // !OBJECT_HPP_
