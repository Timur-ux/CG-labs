#include "objects/Object.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "geometry.hpp"


Object::Object(glm::vec3 position, Program & program) 
  : position_(position)
    , program_(program)
    , model_(1) {
  ProgramBind progBinding(program_);
  VAOBind vaoBingind(vao_);
  
  model_ = glm::translate(model_, position_);
}

void Object::draw() {
  ProgramBind programBinding(program_);

  GLuint modelLoc = glGetUniformLocation(program_.get(), "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
}
