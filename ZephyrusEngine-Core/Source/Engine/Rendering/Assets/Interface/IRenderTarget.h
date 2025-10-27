#pragma once

#include <iostream>
#include "Vector2D.h"

namespace Zephyrus::Render {
	class IRenderTarget
	{
    private:
        virtual void Init() = 0;
        virtual void Destroy() = 0;
    public:
        virtual ~IRenderTarget() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(int pWidth, int pHeight) = 0;

        virtual Vector2D GetDimensions() const = 0;
        virtual unsigned int GetColorTexture() const = 0;
	};
}