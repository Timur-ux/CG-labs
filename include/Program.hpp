#ifndef GL_PROGRAM_HPP_
#define GL_PROGRAM_HPP_

#include <glm/fwd.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
class Program {
  GLuint program_;
  GLuint vshader_;
  GLuint fshader_;

public:
  Program(std::string glslShaderName);
  Program(std::string vshaderFileName, std::string fshaderFileName);

  Program(Program && other);
  Program& operator=(Program && other);

  Program& operator=(Program & other) = delete;
  Program(Program & other) = delete;

  ~Program();

  GLuint get();
  GLint getUniformLoc(const char * name);
  GLint getAttribLoc(const char * name);

  bool setUniformMat4(const char * name, glm::mat4 & data);
  bool setUniformMat3(const char * name, glm::mat3 & data);
  bool setUniformFloat(const char * name, GLfloat & data);
  bool setUniformVec2(const char * name, glm::vec2 & data);
  bool setUniformVec3(const char * name, glm::vec3 & data);
  bool setUniformVec4(const char * name, glm::vec4 & data);

  void bind();
  void unbind();

  static int maximumVertexUniforms();
  static int maximumFragmentUniforms();
};

struct ProgramBind {
  Program & program;
  ProgramBind(Program & programToBind) : program(programToBind) {program.bind();}

  ~ProgramBind() {program.unbind();}
};

#endif // !GL_PROGRAM_HPP_
