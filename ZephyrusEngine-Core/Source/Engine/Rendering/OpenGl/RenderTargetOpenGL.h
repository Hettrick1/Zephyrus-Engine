#pragma once
#include <glew.h>
#include "../Interface/IRenderTarget.h"

namespace Zephyrus::Render {
    class RenderTargetOpenGL : public IRenderTarget {
    private:
        unsigned int mFbo = 0;
        unsigned int mColorTexture = 0;
        unsigned int mDepthTexture = 0;

        int mWidth = 800;
        int mHeight = 600;

    private:
        void Init() override;
        void Destroy() override;
    public:
        RenderTargetOpenGL(int pWidth, int pHeight);
        ~RenderTargetOpenGL() override;

        void Bind() override;
        void Unbind() override;

        void Resize(int pWidth, int pHeight) override;

        inline Vector2D GetDimensions() const override { return Vector2D(mWidth, mHeight); }
        inline unsigned int GetColorTexture() const override { return mColorTexture; }
        unsigned int GetDepthTexture() const override { return mDepthTexture; }
    };
}