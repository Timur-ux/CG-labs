#ifndef GL_CHECK_ERROR_HPP_
#define GL_CHECK_ERROR_HPP_

#include <string>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>


GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void glDebugOutput(GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam);


#endif // !GL_CHECK_ERROR_HPP_
