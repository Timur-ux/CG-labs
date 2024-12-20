#include "Shader.hpp"

#include <fstream>
#include <iostream>


GLuint loadShared(const char * source, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  GLint status, length;
  if(shader == 0) {
    std::cerr << "ERROR: Can't create shader, return 0" << std::endl;
    return 0;
  }

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if(status != GL_TRUE) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char * buf = (char *)malloc(sizeof(GLchar)*length);

    glGetShaderInfoLog(shader, length, NULL, buf);

    std::cerr << "ERROR: Can't compile shader\nSHADER TEXT:\n" << source <<"\nCOMPILE LOG:\n" << std::endl << buf << std::endl;

    free(buf);
    return 0;
  }

  return shader;
}

GLuint loadSharedFromFile(std::string fileName, GLenum shaderType) {
  std::ifstream file(fileName, std::ios::binary | std::ios::ate);
  size_t fileSize = file.tellg();
  file.close();

  char * source = (char *)malloc(sizeof(char)*(fileSize + 1));
  file.open(fileName);
  file.read(source, fileSize);
  source[fileSize] = 0;

  GLuint shader = loadShared(source, shaderType);
  free(source);
  return shader;
} 
