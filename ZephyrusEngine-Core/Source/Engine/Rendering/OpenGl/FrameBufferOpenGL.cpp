#include "pch.h"
#include "FrameBufferOpenGL.h"
#include <ranges>

#include "Log.h"
#include "IRenderer.h"

FrameBufferOpenGL::FrameBufferOpenGL(Zephyrus::Render::IRenderer* renderer)
    : mRendererRef{ renderer }
{
}

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

void FrameBufferOpenGL::AddColorAttachement(AttachementDesc descriptor)
{
    if (mColorAttachements.contains(descriptor.mName))
    {
        mColorAttachements.erase(descriptor.mName);
    }
    unsigned int colorAttachement = 0; // TODO : get the texture from the renderer
    FrameBufferTextureDesc desc = FrameBufferTextureDesc{ descriptor, colorAttachement };
    mColorAttachements[descriptor.mName] = desc;
}

void FrameBufferOpenGL::AddDepthAttachement(FramebufferTextureFormat format)
{
    unsigned int depthAttachement = 0; // TODO : get the texture from the renderer
    AttachementDesc descriptor = AttachementDesc("depth", format);
    FrameBufferTextureDesc desc = FrameBufferTextureDesc{ descriptor, depthAttachement };
    mDepthAttachement = desc;
}

void FrameBufferOpenGL::RemoveColorAttachement(const std::string& name)
{
    if (mColorAttachements.contains(name))
    {
        glDeleteTextures(1, &mColorAttachements[name].mTextureAttachement);
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
    std::vector<GLenum> drawBuffers;
    for (auto const& desc : std::views::values(mColorAttachements))
    {
        glBindTexture(GL_TEXTURE_2D, desc.mTextureAttachement);

        unsigned int format = 0;
        switch (desc.mDescriptor.mFormat)
        {
        case FramebufferTextureFormat::RGBA8:
            format = GL_RGBA8;
            break;
        default:
            format = GL_RGBA8;
            break;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, desc.mTextureAttachement, 0);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
        index++;
    }

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    if (mDepthAttachement.mTextureAttachement != 0)
    {
        unsigned int internalFormat = 0;
        switch (mDepthAttachement.mDescriptor.mFormat)
        {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            internalFormat = GL_DEPTH24_STENCIL8;
            break;
        default:
            internalFormat = GL_DEPTH24_STENCIL8;
            break;
        }
        
        glBindTexture(GL_TEXTURE_2D, mDepthAttachement.mTextureAttachement);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachement.mTextureAttachement, 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ZP_CORE_ERROR("RenderTarget : Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferOpenGL::Destroy()
{
    if (mDepthAttachement.mTextureAttachement) { glDeleteTextures(1, &mDepthAttachement.mTextureAttachement); mDepthAttachement.mTextureAttachement = 0; }
    if (mFboHandle) { glDeleteFramebuffers(1, &mFboHandle); mFboHandle = 0; }
}

void FrameBufferOpenGL::DestroyAttachements()
{
    if (!mColorAttachements.empty())
    {
        for (auto const& desc : std::views::values(mColorAttachements))
        {
            glDeleteTextures(1, &desc.mTextureAttachement);
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
