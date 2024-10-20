#ifndef GL_PROGRAM_HPP_
#define GL_PROGRAM_HPP_

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
class Program {
  GLuint program_;
public:
  Program(std::string vshaderFileName, std::string fshaderFileName);

  Program(Program && other);
  Program& operator=(Program && other);

  Program& operator=(Program & other) = delete;
  Program(Program & other) = delete;

  ~Program();

  GLuint get();

  void bind();
  void unbind();
};

struct ProgramBind {
  Program & program;
  ProgramBind(Program & programToBind) : program(programToBind) {program.bind();}

  ~ProgramBind() {program.unbind();}
};

#endif // !GL_PROGRAM_HPP_
