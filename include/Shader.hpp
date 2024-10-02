#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

GLuint loadSharedFromFile(std::string fileName, GLenum shaderType); 


#endif // !SHADER_HPP_
