#ifndef GL_CHECK_ERROR_HPP_
#define GL_CHECK_ERROR_HPP_

#define GLEW_STATIC
#include <GL/glew.h>


GLenum glCheckError_(const char *file, int line);
#ifdef DEBUG
#define glCheckError() glCheckError_(__FILE__, __LINE__)
#else
#define glCheckError() {};
#endif

void glDebugOutput(GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam);


#endif // !GL_CHECK_ERROR_HPP_
