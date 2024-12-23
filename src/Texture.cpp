#include "Texture.hpp"
#include "glCheckError.hpp"
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <iostream>

// Texture
// ----------

void Texture::bind() {
  glCheckError();
  glActiveTexture(GL_TEXTURE0 + textureBlock_);
  glCheckError();
  glBindTexture(GL_TEXTURE_2D, texture_);
  glCheckError();
}

void Texture::unbind() {
  glActiveTexture(GL_TEXTURE0 + textureBlock_);
  glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::maximumTextureBlocks() {
  int res;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &res);

  return res;
}


Texture::Texture(Texture && other) {
  texture_ = other.texture_;
  other.texture_ = 0;
}

Texture & Texture::operator=(Texture && other) {
  if(this == &other)
    return *this;

  texture_ = other.texture_;
  other.texture_ = 0;

  return *this;
}

Texture::~Texture() {
  if(texture_ != 0)
    glDeleteTextures(1, &texture_);
}

// Texture 2D
// ----------

Texture2D::Texture2D(std::string textureFileName, int block, GLenum imageType, int soilLoadType) : Texture() {
  textureBlock_ = block;
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  int width, height;

  unsigned char * data = SOIL_load_image(textureFileName.c_str(), &width, &height, 0, soilLoadType);
  width_ = width, height_= height;
  if(data == nullptr)
    std::cerr << "Can't load image from file: " << textureFileName << std::endl
      << "Soil last result: " << SOIL_last_result() << std::endl;

  glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}


Texture2D::Texture2D(int width, int height, GLenum type, std::list<std::pair<GLenum, GLint>> texParameters, GLenum elementType, int block) : Texture(), width_(width), height_(height) {
  textureBlock_ = block;
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, elementType, nullptr);

  for(auto & [param, value] : texParameters) {
    glTexParameteri(GL_TEXTURE_2D, param, value);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glCheckError();
}

Texture2D::Texture2D(int width, int height, unsigned char * data, GLenum imageType, int block) : Texture() {
  textureBlock_ = block;
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  width_ = width, height_= height;
  glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  glCheckError();
}

Texture2D * Texture2D::createMonocolor(int width, int height, glm::vec3 color, int block) {
  unsigned char data[width][height][4];
  for(int i = 0; i < width; ++i) {
    for(int j = 0; j < height; ++j) {
        data[i][j][0] = 255*color.r;
        data[i][j][1] = 255*color.g;
        data[i][j][2] = 255*color.b;
        data[i][j][3] = 255;
    }
  }

  return new Texture2D(width, height, (unsigned char *)data, GL_RGBA, block);
}

Texture2D::Texture2D(Texture2D && other) {
  texture_ = other.texture_;
  other.texture_ = 0;
  textureBlock_ = other.textureBlock_;
  other.textureBlock_ = -1;
}

Texture2D & Texture2D::operator=(Texture2D && other) {
  if(this == &other)
    return *this;

  texture_ = other.texture_;
  other.texture_ = 0;
  textureBlock_ = other.textureBlock_;
  other.textureBlock_ = -1;

  return *this;
}

bool Texture::setTextureBlock(GLint newBlock) {
  textureBlock_ = newBlock;
  return true;
}
