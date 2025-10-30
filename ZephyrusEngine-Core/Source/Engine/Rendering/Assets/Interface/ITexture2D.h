#pragma once

#include "ITextureBase.h"

namespace Zephyrus::Assets 
{
    class ITexture2D : public ITextureBase
    {
    public:
        virtual ~ITexture2D() = default;

        virtual bool Load(const std::string& pFilename) = 0;

        virtual std::string GetFilePath() const = 0;
    };
}
