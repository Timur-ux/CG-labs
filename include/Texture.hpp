#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>


/*
 * Creates 2d texture with rgba image
 * */
class Texture2D {
  GLuint texture_;
  int width_;
  int height_;

  public:
  Texture2D(std::string textureFileName);
  
  Texture2D(Texture2D & other) = delete;
  Texture2D & operator=(Texture2D & other) = delete;

  Texture2D(Texture2D && other);
  Texture2D & operator=(Texture2D && other);

  ~Texture2D();

  void bind();
  void unbind();

  int width() {return width_;}
  int height() {return height_;}

};

#endif // !TEXTURE_HPP_
