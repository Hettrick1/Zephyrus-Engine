#pragma once

#include "../IFrameBuffer.h"

class FrameBufferOpenGL : public IFrameBuffer
{
private:
    std::unordered_map<std::string, std::shared_ptr<TextureHandle>> mColorAttachements;
    unsigned int mDepthAttachement = 0;
    unsigned int mFboHandle = 0;

    int mWidth = 800;
    int mHeight = 600;
    
public:
    ~FrameBufferOpenGL() override;

    void Bind() override;
    void UnBind() override;

    unsigned int GetHandle() const override { return mFboHandle; }
    
    TextureHandle* GetColorAttachement(const std::string& name) override
    {
        if (mColorAttachements.contains(name))
        {
            return mColorAttachements[name].get(); // TODO : Return shared nopt raw
        }
        return nullptr;
    }
    void AddColorAttachement(const std::string& name) override;
    void RemoveColorAttachement(const std::string& name) override;
    unsigned int GetDepthAttachement() override { return mDepthAttachement; }

    void Init() override;
    void Destroy() override;
    void DestroyAttachements();

    void Resize(int pWidth, int pHeight) override;
    Vector2D GetDimensions() const override { return Vector2D(mWidth, mHeight); }
};
