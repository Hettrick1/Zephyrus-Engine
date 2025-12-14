#pragma once

#include "../IFrameBuffer.h"

class FrameBufferOpenGL : public IFrameBuffer
{
private:
    std::unordered_map<std::string, TextureHandle*> mColorAttachements;
    unsigned int mDepthAttachement;
    unsigned int mFboHandle;

    int mWidth = 800;
    int mHeight = 600;
    
public:
    ~FrameBufferOpenGL() override = default;

    void Bind() override;
    void UnBind() override;

    void GetHandle() override;

    std::unordered_map<std::string, TextureHandle*> GetAllColorAttachements() override { return mColorAttachements; }
    TextureHandle* GetColorAttachement(const std::string& name) override
    {
        if (mColorAttachements.contains(name))
        {
            return mColorAttachements[name];
        }
        return nullptr;
    }
    void AddColorAttachement(const std::string& name) override;
    void RemoveColorAttachement(const std::string& name) override;
    unsigned int GetDepthAttachement() override { return mDepthAttachement; }

    void Init() override;
    void Destroy() override;

    void Resize(int pWidth, int pHeight) override;
    Vector2D GetDimensions() const override { return Vector2D(mWidth, mHeight); }
};
