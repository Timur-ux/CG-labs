#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "Texture.hpp"
#include "glCheckError.hpp"
#include <iostream>
#include <GL/glew.h>

struct FBOBind;
class Framebuffer {
  private:
    GLuint fbo_;
    int width_, height_, flags_;

    Texture2D * depthBuffer_ = nullptr;
    Texture2D * stencilBuffer_ = nullptr;
    Texture2D * colorBuffers_[8] {nullptr};
    size_t colorBuffersCounter_ = 0;

    GLenum target_;
    GLint saveViewPort_[4];

  public:
    Framebuffer(int width, int height, int flags, GLenum target = GL_FRAMEBUFFER);
    Framebuffer(Framebuffer &) = delete;
    Framebuffer(Framebuffer &&) noexcept;

    Framebuffer &operator=(Framebuffer &) = delete;
    Framebuffer & operator=(Framebuffer &&) noexcept;

    GLenum target() const {return target_;}
    int width() const {return width_;}
    int height() const {return height_;}
    Texture2D * depthBuffer() {return depthBuffer_;}

    void bind();
    void unbind();

    GLenum checkStatus();
    bool isOk();

    /**
     * @brief bind texture as depth buffer
     *
     * @param depthBuffer -- must be dynamically allocated, with GL_FLOAT elementType
     */
    void bindDepthBuffer(Texture2D * depthBuffer);

    /**
     * @brief crete new texture and bind as depth buffer
     * 
     * @param block -- texture block where texture stores is
     */
    void createDepthBuffer(int block);

    /**
     * @brief bind texture as stencil buffer
     *
     * @param stencilBuffer -- must be dynamically allocated
     */
    void bindStencil(Texture2D * stencilBuffer);

    /**
     * @brief crete new texture and bind as stencil buffer
     * 
     */
    void createStencilBuffer();

    /**
     * @brief bind texture as color buffer
     *
     * @param colorBuffer -- must be dynamically allocated
     * @param i -- index of buffer
     */
    void bindColorBuffer(Texture2D * colorBuffer, size_t i);
    /**
     * @brief crete new texture and bind as color buffer
     *
     * @param i -- index of buffer (must be in [0, 7])
     */
    void createColorBuffer(size_t i);

    ~Framebuffer();

    static Framebuffer&& createShadowMap(int width, int height, int depthBufferTextureBlock, int flags = 0) {
      static Framebuffer framebuffer = Framebuffer(width, height, 0);

      framebuffer = Framebuffer(width, height, 0);
      glCheckError();
      framebuffer.createDepthBuffer(depthBufferTextureBlock);
      glCheckError();
      framebuffer.bind();
      glCheckError();
      if(!framebuffer.isOk()) 
        std::cerr << "Error: framebuffer is not complete" << std::endl;
      else 
        std::cerr << "Info: framebuffer is good!" << std::endl;
      framebuffer.unbind();
      glCheckError();

      return std::move(framebuffer);
    }
};

struct FBOBind {
  Framebuffer & framebuffer;
  GLuint oldFramebuffer;

  FBOBind(Framebuffer & framebufferToBind) : framebuffer(framebufferToBind) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&oldFramebuffer);
    framebuffer.bind();
  }

  ~FBOBind() {
    glBindFramebuffer(framebuffer.target(), oldFramebuffer);
  }
};


#endif // !FRAMEBUFFER_HPP_
