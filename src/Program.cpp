#include "Program.hpp"
#include "Shader.hpp"
#include <stdexcept>

Program::Program(std::string vshaderFileName, std::string fshaderFileName) {
  GLuint vshader = loadSharedFromFile(vshaderFileName, GL_VERTEX_SHADER); 
  if(!vshader) {
    throw std::runtime_error("Can't load vertex shader from file: " + vshaderFileName);
  }

  
  GLuint fshader = loadSharedFromFile(fshaderFileName, GL_FRAGMENT_SHADER); 
  if(!fshader) {
    throw std::runtime_error("Can't load fragment shader from file: " + fshaderFileName);
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vshader);
  glAttachShader(program_, fshader);
  glLinkProgram(program_);

  GLint status;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);

  if(status != GL_TRUE) {
    GLint length;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);


    char * buf = (char *)malloc(sizeof(GLchar)*length);

    glGetProgramInfoLog(program_, length, NULL, buf);

    throw std::runtime_error("ERROR: Can't compile shader, compile log:\n" + std::string(buf) + '\n');
    free(buf);
  }
  glDeleteShader(vshader);
  glDeleteShader(fshader);
}

Program::Program(Program &&other) {
  program_ = other.program_;
  other.program_ = 0;
}


Program & Program::operator=(Program && other) {
  if(this == &other)
    return *this;
  program_ = other.program_;
  other.program_ = 0;

  return *this;
}

Program::~Program() {
  glDeleteProgram(program_);
}


void Program::bind() {
  glUseProgram(program_);
}

void Program::unbind() {
  glUseProgram(0);
}

GLuint Program::get() {
  return program_;
}
