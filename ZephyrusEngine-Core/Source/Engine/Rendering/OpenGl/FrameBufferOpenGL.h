#pragma once

#include "../IFrameBuffer.h"

namespace Zephyrus::Render
{
    class IRenderer;
}

struct FrameBufferTextureDesc
{
    FrameBufferTextureDesc();
    FrameBufferTextureDesc(const AttachementDesc& pDesc, unsigned int pAttachment);
    AttachementDesc mDescriptor = AttachementDesc("none", FramebufferTextureFormat::None);
    unsigned int mTextureAttachement = 0;
};

class FrameBufferOpenGL : public IFrameBuffer
{
private:
    std::unordered_map<std::string, FrameBufferTextureDesc> mColorAttachements;
    FrameBufferTextureDesc mDepthAttachement;
    unsigned int mFboHandle = 0;

    int mWidth = 800;
    int mHeight = 600;

    Zephyrus::Render::IRenderer* mRendererRef{nullptr}; // TODO : Give only an interface, not the all renderer as we don't need everything
    
public:
    FrameBufferOpenGL(Zephyrus::Render::IRenderer* renderer);
    ~FrameBufferOpenGL() override;

    void Bind() override;
    void UnBind() override;

    unsigned int GetHandle() const override { return mFboHandle; }
    
    unsigned int GetColorAttachement(const std::string& name) override
    {
        if (mColorAttachements.contains(name))
        {
            return mColorAttachements[name].mTextureAttachement; // TODO : Return shared nopt raw
        }
        return 0;
    }
    void AddColorAttachement(AttachementDesc descriptor) override;
    void AddDepthAttachement(FramebufferTextureFormat format) override;
    void RemoveColorAttachement(const std::string& name) override;
    unsigned int GetDepthAttachement() override { return mDepthAttachement.mTextureAttachement; }

    void Init() override;
    void Destroy() override;
    void DestroyAttachements();

    void Resize(int pWidth, int pHeight) override;
    Vector2D GetDimensions() const override { return Vector2D(mWidth, mHeight); }
};
