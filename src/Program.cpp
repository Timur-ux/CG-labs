#include "Program.hpp"
#include "Shader.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

GLuint getShader(std::string source, std::string type) {
  GLuint result = 0;
  if (type == "vertex") {
    result = loadShared(source.c_str(), GL_VERTEX_SHADER);
  } else if (type == "fragment") {
    result = loadShared(source.c_str(), GL_FRAGMENT_SHADER);
  } else
    throw std::invalid_argument("Undefined shader type: " + type);

  return result;
};

Program::Program(std::string glslShaderName) {
  std::stringstream vertexShaderBuffer, fragmentShaderBuffer;
  std::string temp;
  std::string type;
  std::ifstream file(glslShaderName);

  while (std::getline(file, temp)) {
    if (temp.substr(0, 2) == "--") {
      type = temp.substr(3, temp.npos);
      continue;
    }
    if (type == "vertex")
      vertexShaderBuffer << temp << '\n';
    if (type == "fragment")
      fragmentShaderBuffer << temp << '\n';
  }

  GLuint vshader =
      loadShared(vertexShaderBuffer.str().c_str(), GL_VERTEX_SHADER);
  if (!vshader) {
    throw std::runtime_error("Can't load vertex shader from file: " +
                             glslShaderName);
  }

  GLuint fshader =
      loadShared(fragmentShaderBuffer.str().c_str(), GL_FRAGMENT_SHADER);
  if (!fshader) {
    throw std::runtime_error("Can't load fragment shader from file: " +
                             glslShaderName);
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vshader);
  glAttachShader(program_, fshader);
  glLinkProgram(program_);

  GLint status;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);

  if (status != GL_TRUE) {
    GLint length;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);

    char *buf = (char *)malloc(sizeof(GLchar) * length);

    glGetProgramInfoLog(program_, length, NULL, buf);

    throw std::runtime_error("ERROR: Can't compile shader, compile log:\n" +
                             std::string(buf) + '\n');
    free(buf);
  }
  vshader_ = vshader;
  fshader_ = fshader;
}

Program::Program(std::string vshaderFileName, std::string fshaderFileName) {
  GLuint vshader = loadSharedFromFile(vshaderFileName, GL_VERTEX_SHADER);
  if (!vshader) {
    throw std::runtime_error("Can't load vertex shader from file: " +
                             vshaderFileName);
  }

  GLuint fshader = loadSharedFromFile(fshaderFileName, GL_FRAGMENT_SHADER);
  if (!fshader) {
    throw std::runtime_error("Can't load fragment shader from file: " +
                             fshaderFileName);
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vshader);
  glAttachShader(program_, fshader);
  glLinkProgram(program_);

  GLint status;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);

  if (status != GL_TRUE) {
    GLint length;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);

    char *buf = (char *)malloc(sizeof(GLchar) * length);

    glGetProgramInfoLog(program_, length, NULL, buf);

    throw std::runtime_error("ERROR: Can't compile shader, compile log:\n" +
                             std::string(buf) + '\n');
    free(buf);
  }
  vshader_ = vshader;
  fshader_ = fshader;
}

Program::Program(Program &&other) {
  program_ = other.program_;
  other.program_ = 0;
}

Program &Program::operator=(Program &&other) {
  if (this == &other)
    return *this;
  program_ = other.program_;
  other.program_ = 0;

  return *this;
}

Program::~Program() { glDeleteProgram(program_); }

void Program::bind() { glUseProgram(program_); }

void Program::unbind() { glUseProgram(0); }

GLuint Program::get() { return program_; }

GLint Program::getUniformLoc(const char *name) {
  return glGetUniformLocation(program_, name);
}
GLint Program::getAttribLoc(const char * name) {
  return glGetAttribLocation(program_, name);
}

bool Program::setUniformMat4(const char *name, glm::mat4 &data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data));
  return true;
}

bool Program::setUniformMat3(const char *name, glm::mat3 &data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(data));
  return true;
}

bool Program::setUniformFloat(const char *name, GLfloat data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniform1f(loc, data);
  return true;
}

bool Program::setUniformInt(const char *name, GLint data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniform1i(loc, data);
  return true;
}

bool Program::setUniformVec2(const char *name, glm::vec2 &data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniform2fv(loc, 1, glm::value_ptr(data));
  return true;
}

bool Program::setUniformVec3(const char *name, glm::vec3 &data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniform3fv(loc, 1, glm::value_ptr(data));
  return true;
}

bool Program::setUniformVec4(const char *name, glm::vec4 &data) {
  ProgramBind _(*this);
  GLint loc = getUniformLoc(name);
  if (loc < 0)
    return false;
  glUniform4fv(loc, 1, glm::value_ptr(data));
  return true;
}


bool Program::setUniformMat4(GLint loc, glm::mat4 &data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data));
  return true;
}

bool Program::setUniformMat3(GLint loc, glm::mat3 &data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(data));
  return true;
}

bool Program::setUniformFloat(GLint loc, GLfloat data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniform1f(loc, data);
  return true;
}

bool Program::setUniformInt(GLint loc, GLint data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniform1i(loc, data);
  return true;
}

bool Program::setUniformVec2(GLint loc, glm::vec2 &data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniform2fv(loc, 1, glm::value_ptr(data));
  return true;
}

bool Program::setUniformVec3(GLint loc, glm::vec3 &data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniform3fv(loc, 1, glm::value_ptr(data));
  return true;
}

bool Program::setUniformVec4(GLint loc, glm::vec4 &data) {
  ProgramBind _(*this);
  if (loc < 0)
    return false;
  glUniform4fv(loc, 1, glm::value_ptr(data));
  return true;
}

int Program::maximumVertexUniforms() {
  GLint res;
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &res);
  return res;
}

int Program::maximumFragmentUniforms() {
  GLint res;
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &res);
  return res;
}
