#ifndef GL_PROGRAM_HPP_
#define GL_PROGRAM_HPP_

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
class Program {
  GLuint program_;

public:
  Program(std::string vshaderFileName, std::string fshaderFileName);

  GLuint get();

  void bind();
  void unbind();
};

#endif // !GL_PROGRAM_HPP_
