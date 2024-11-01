#include "Framebuffer.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include <cassert>
#include <stdexcept>

Framebuffer::Framebuffer(int width, int height, int flags, GLenum target)
    : width_(width), height_(height), flags_(flags), target_(target) {
  glGenFramebuffers(1, &fbo_);
}

Framebuffer::Framebuffer(Framebuffer &&other) noexcept {
  fbo_ = other.fbo_;
  other.fbo_ = 0;

  depthBuffer_ = other.depthBuffer_;
  other.depthBuffer_ = 0;

  stencilBuffer_ = other.depthBuffer_;
  other.stencilBuffer_ = 0;

  for (size_t i = 0; i < 8; ++i) {
    colorBuffers_[i] = other.colorBuffers_[i];
    other.colorBuffers_[i] = 0;
  }

  width_ = other.width_;
  height_ = other.height_;
  flags_ = other.flags_;
}

Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept {
  if (this != &other) {
    fbo_ = other.fbo_;
    other.fbo_ = 0;

    depthBuffer_ = other.depthBuffer_;
    other.depthBuffer_ = 0;

    stencilBuffer_ = other.depthBuffer_;
    other.stencilBuffer_ = 0;

    for (size_t i = 0; i < 8; ++i) {
      colorBuffers_[i] = other.colorBuffers_[i];
      other.colorBuffers_[i] = 0;
    }

    width_ = other.width_;
    height_ = other.height_;
    flags_ = other.flags_;
  }

  return *this;
}

Framebuffer::~Framebuffer() {
  if (fbo_ != 0)
    glDeleteFramebuffers(1, &fbo_);
  if (depthBuffer_)
    delete depthBuffer_;
  if (stencilBuffer_)
    delete stencilBuffer_;
  for (size_t i = 0; i < 8; ++i) {
    if (colorBuffers_[i])
      delete colorBuffers_[i];
  }
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glGetIntegerv(GL_VIEWPORT, (GLint*)&saveViewPort_);

  glViewport(0, 0, width_, height_);
  if(colorBuffersCounter_ == 0) {
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);
  } else {
    // TODO
  }
  if(depthBuffer_) {
    depthBuffer_->bind();
  }
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if(depthBuffer_)
    depthBuffer_->unbind();

  glViewport(saveViewPort_[0], saveViewPort_[1], saveViewPort_[2], saveViewPort_[3]);
}

GLenum Framebuffer::checkStatus() {
  FBOBind _(*this);
  return glCheckFramebufferStatus(target_);
}

bool Framebuffer::isOk() {
  FBOBind _(*this);
  return checkStatus() == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::bindDepthBuffer(Texture2D *depthBuffer) {
  FBOBind _(*this);
  glCheckError();
  depthBuffer_ = depthBuffer;
  glCheckError();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthBuffer->texture_, 0);
  std::cout << "FBO: " << fbo_ << std::endl;
  glCheckError();
}

void Framebuffer::createDepthBuffer(int block) {
  FBOBind _(*this);
  glCheckError();
  depthBuffer_ = new Texture2D(width_, height_, GL_DEPTH_COMPONENT,
                               {
                                   {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
                                   {GL_TEXTURE_MIN_FILTER, GL_LINEAR},
                                   {GL_TEXTURE_WRAP_S, GL_REPEAT},
                                   {GL_TEXTURE_WRAP_T, GL_REPEAT},
                               },
                               GL_FLOAT, block);
  glCheckError();
  bindDepthBuffer(depthBuffer_);
  glCheckError();
}

void Framebuffer::bindStencil(Texture2D *stencilBuffer) {
  FBOBind _(*this);
  // TODO
}

void Framebuffer::createStencilBuffer() {
  FBOBind _(*this);
  // TODO
}

void Framebuffer::bindColorBuffer(Texture2D *colorBuffer, size_t i) {
  if (i < 0 || i > 7)
    throw std::invalid_argument("i must be in [0, 7] slice");
  FBOBind _(*this);

  // TODO
}

void Framebuffer::createColorBuffer(size_t i) {
  if (i < 0 || i > 7)
    throw std::invalid_argument("i must be in [0, 7] slice");
  FBOBind _(*this);

  // TODO
}
