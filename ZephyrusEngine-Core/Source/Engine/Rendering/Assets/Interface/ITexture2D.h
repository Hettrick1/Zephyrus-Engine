#pragma once

#include "ITextureBase.h"

namespace Zephyrus::Assets 
{
    class ITexture2D : public ITextureBase
    {
    public:
        virtual ~ITexture2D() override = default;

        virtual bool Load(const std::string& pFilename, const std::string& id) = 0;

        virtual std::string GetTextureFileId() const = 0;
    };
}
