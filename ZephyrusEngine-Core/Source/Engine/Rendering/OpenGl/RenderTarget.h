#pragma once
#include <glew.h>
#include <iostream>
#include "Vector2D.h"

namespace Zephyrus::Render {
    class RenderTarget {
    private:
        unsigned int mFbo = 0;
        unsigned int mColorTexture = 0;
        unsigned int mDepthTexture = 0;

        int mWidth = 800;
        int mHeight = 600;

    private:
        void Init();
        void Destroy();
    public:
        RenderTarget(int pWidth, int pHeight);
        ~RenderTarget();

        void Bind();
        static void Unbind();

        void Resize(int pWidth, int pHeight);

        inline Vector2D GetDimensions() const { return Vector2D(mWidth, mHeight); }
        inline unsigned int GetColorTexture() const { return mColorTexture; }
    };
}