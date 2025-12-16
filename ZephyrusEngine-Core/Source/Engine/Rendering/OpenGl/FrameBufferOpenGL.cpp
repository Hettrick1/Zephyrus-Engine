#include "pch.h"
#include "FrameBufferOpenGL.h"
#include <ranges>

#include "Log.h"

FrameBufferOpenGL::~FrameBufferOpenGL()
{
    DestroyAttachements();
    FrameBufferOpenGL::Destroy();
}

void FrameBufferOpenGL::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);
    glViewport(0, 0, mWidth, mHeight);
}

void FrameBufferOpenGL::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferOpenGL::AddColorAttachement(const std::string& name)
{
    if (mColorAttachements.contains(name))
    {
        glDeleteTextures(1, &mColorAttachements[name]->mHandle);
    }
    std::shared_ptr<TextureHandle> texture = std::make_shared<TextureHandle>();
    glGenTextures(1, &texture->mHandle);
    glBindTexture(GL_TEXTURE_2D, texture->mHandle);
    mColorAttachements[name] = texture;
    Init();
}

void FrameBufferOpenGL::RemoveColorAttachement(const std::string& name)
{
    if (mColorAttachements.contains(name))
    {
        glDeleteTextures(1, &mColorAttachements[name]->mHandle);
    }
    mColorAttachements.erase(name);
}

void FrameBufferOpenGL::Init()
{
    if (mColorAttachements.empty())
    {
        return;
    }
    glGenFramebuffers(1, &mFboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);

    unsigned int index = 0;
    for (auto const& texture : std::views::values(mColorAttachements))
    {
        glBindTexture(GL_TEXTURE_2D, texture->mHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
            GL_TEXTURE_2D, texture->mHandle, 0);
        index++;
    }

    glGenTextures(1, &mDepthAttachement);
    glBindTexture(GL_TEXTURE_2D, mDepthAttachement);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D, mDepthAttachement, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ZP_CORE_ERROR("RenderTarget : Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferOpenGL::Destroy()
{
    if (mDepthAttachement) { glDeleteTextures(1, &mDepthAttachement); mDepthAttachement = 0; }
    if (mFboHandle) { glDeleteFramebuffers(1, &mFboHandle); mFboHandle = 0; }
}

void FrameBufferOpenGL::DestroyAttachements()
{
    if (!mColorAttachements.empty())
    {
        for (auto& c : mColorAttachements)
        {
            glDeleteTextures(1, &c.second->mHandle);
        }
        mColorAttachements.clear();
    }
}

void FrameBufferOpenGL::Resize(int pWidth, int pHeight)
{
    if (pWidth == mWidth && pHeight == mHeight) return;
    mWidth = pWidth;
    mHeight = pHeight;
    Destroy();
    Init();
}
