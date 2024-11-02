#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <SOIL/SOIL.h>
#include <list>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

class Framebuffer;
namespace my {
class Texture {
protected:
  GLuint texture_ = 0;
  GLint textureBlock_ = 0;
  Texture() = default;

  friend Framebuffer;

public:
  Texture(Texture &other) = delete;
  Texture &operator=(Texture &other) = delete;

  Texture(Texture &&other);
  Texture &operator=(Texture &&other);

  GLint block() { return textureBlock_; }
  GLuint texture() { return texture_; }
  bool setTextureBlock(GLint indexOfNewBlock);

  static int maximumTextureBlocks();
  void bind();
  void unbind();

  virtual ~Texture();
};

}

/*
 * Creates 2d texture with rgba image
 * */
class Texture2D : public my::Texture {
  int width_;
  int height_;

public:
  Texture2D(std::string textureFileName, int block = 0,
            GLenum imageType = GL_RGB, int soilLoadType = SOIL_LOAD_RGB);

  /**
   * @brief create raw texture without image
   *
   * @param width
   * @param height
   * @param type
   * GL_RGB/GL_DEPTH_COMPONENT/GL_STENCIL_COMPONENT/GL_DEPTH_STENCIL_INDEX
   * @param elementType GL_UNSIGNED_BYTE
   */
  Texture2D(int width, int height, GLenum type,
            std::list<std::pair<GLenum, GLint>> texParameters =
                {{GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                 {GL_TEXTURE_MIN_FILTER, GL_NEAREST}},
            GLenum elementType = GL_UNSIGNED_BYTE, int block = 0);
  Texture2D(unsigned char *data, int block = 0);

  Texture2D(Texture2D &other) = delete;
  Texture2D &operator=(Texture2D &other) = delete;

  Texture2D(Texture2D &&other);
  Texture2D &operator=(Texture2D &&other);

  int width() { return width_; }
  int height() { return height_; }
};

#endif // !TEXTURE_HPP_
