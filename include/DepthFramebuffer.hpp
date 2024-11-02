#ifndef DEPTH_FRAMEBUFFER_HPP_
#define DEPTH_FRAMEBUFFER_HPP_

#include "Program.hpp"
#include "Texture.hpp"
#include "glCheckError.hpp"
#include <GL/glew.h>
#include <iostream>

class DepthFramebuffer {
  GLuint fbo_;
  int width_;
  int height_;
  Texture2D *depthMap_;
  GLuint saveViewport_[4];

public:
  GLuint depthTex_;
  int depthTexBlock_;
  DepthFramebuffer(int width, int height, int depthTexBlock)
      : width_(width), height_(height), depthMap_(nullptr), depthTexBlock_(depthTexBlock) {
    glGenFramebuffers(1, &fbo_);
    depthMap_ = new Texture2D(width_, height_, GL_DEPTH_COMPONENT,
                              {
                                  {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                  {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                  {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER},
                                  {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER},
                                  {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER},
                              },
                              GL_FLOAT, depthTexBlock);
    depthTex_ = depthMap_->texture();

    depthMap_->bind();
    // glGenTextures(1, &depthTex_);
    // glCheckError();
    // glBindTexture(GL_TEXTURE_2D, depthTex_);
    // glCheckError();
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // glCheckError();
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glCheckError();
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glCheckError();
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glCheckError();
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glCheckError();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthMap_->texture(), 0);
    glCheckError();
    glDrawBuffer(GL_NONE);
    glCheckError();
    glReadBuffer(GL_NONE);
    glCheckError();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cerr << "Depth framebuffer is incomplete" << std::endl;
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
  }

  DepthFramebuffer(DepthFramebuffer &) = delete;
  DepthFramebuffer & operator=(DepthFramebuffer &) = delete;
  DepthFramebuffer(DepthFramebuffer && other) {
    fbo_ = other.fbo_;
    depthMap_ = other.depthMap_;
    width_ = other.width_;
    height_ = other.height_;

    other.fbo_ = 0;
    other.depthMap_ = nullptr;

    depthTexBlock_ = other.depthTexBlock_;
    depthTex_ = other.depthTex_;
    other.depthTexBlock_ = 0;
    other.depthTex_ = 0;
  }
  DepthFramebuffer & operator=(DepthFramebuffer && other) {
    if(this == &other)
      return *this;

    fbo_ = other.fbo_;
    depthMap_ = other.depthMap_;
    width_ = other.width_;
    height_ = other.height_;

    other.fbo_ = 0;
    other.depthMap_ = nullptr;

    depthTexBlock_ = other.depthTexBlock_;
    depthTex_ = other.depthTex_;
    other.depthTexBlock_ = 0;
    other.depthTex_ = 0;

    return *this;
  }

  ~DepthFramebuffer() {
    if(fbo_)
      glDeleteFramebuffers(1, &fbo_);
    if(depthMap_)
      delete depthMap_;
  }

  void bind() {
    glGetIntegerv(GL_VIEWPORT, (GLint *)&saveViewport_);
    glViewport(0, 0, width_, height_);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  }

  void unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(saveViewport_[0], saveViewport_[1], saveViewport_[2],
               saveViewport_[3]);
  }

  bool setDepthTexAsUniform(Program & program, const char * name) {
    bool res = program.setUniformInt(name, depthTexBlock_);
    glCheckError();
    return res;
  }

  bool setDepthTexAsUniform(Program & program, GLint loc) {
    bool res = program.setUniformInt(loc, depthTexBlock_);
    glCheckError();
    return res;
  }

  void bindDepthMap(int block) {
    depthMap_->setTextureBlock(block);
    depthMap_->bind();
  }
  void unbindDepthMap(int block) {
    depthMap_->setTextureBlock(block);
    depthMap_->unbind();
  }
};

#endif // !DEPTH_FRAMEBUFFER_HPP_
