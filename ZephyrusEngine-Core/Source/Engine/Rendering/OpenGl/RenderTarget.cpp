#include "RenderTarget.h"
#include "Log.h"

namespace Zephyrus::Render {
    RenderTarget::RenderTarget(int pWidth, int pHeight)
        : mWidth(pWidth), mHeight(pHeight)
    {
        Init();
    }
    RenderTarget::~RenderTarget()
    {
        Destroy();
    }

    void RenderTarget::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
        glViewport(0, 0, mWidth, mHeight);
    }

    void RenderTarget::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Resize(int pWidth, int pHeight)
    {
        if (pWidth == mWidth && pHeight == mHeight) return;
        mWidth = pWidth;
        mHeight = pHeight;
        Destroy();
        Init();
    }

    void RenderTarget::Init()
    {
        glGenFramebuffers(1, &mFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

        glGenTextures(1, &mColorTexture);
        glBindTexture(GL_TEXTURE_2D, mColorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mColorTexture, 0);

        glGenTextures(1, &mDepthTexture);
        glBindTexture(GL_TEXTURE_2D, mDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, mDepthTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            ZP_CORE_ERROR("ERROR::RenderTarget:: Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Destroy()
    {
        if (mDepthTexture) { glDeleteTextures(1, &mDepthTexture); mDepthTexture = 0; }
        if (mColorTexture) { glDeleteTextures(1, &mColorTexture); mColorTexture = 0; }
        if (mFbo) { glDeleteFramebuffers(1, &mFbo); mFbo = 0; }
    }
}

