#ifndef CUSTOM_WINDOW_HPP_
#define CUSTOM_WINDOW_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief Set default openGL context params
 * Double buffer, resizible, openGL version, etc
 */
void setDefaultCtxParams();

#endif // !CUSTOM_WINDOW_HPP_
