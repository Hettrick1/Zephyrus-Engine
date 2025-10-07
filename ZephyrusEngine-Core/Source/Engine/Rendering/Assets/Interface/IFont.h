#pragma once

#include "Vector2D.h"
#include <string>
#include <map>

namespace Zephyrus::Assets {

    struct CharacterInfo
    {
        Vector2D Size = Vector2D::Zero;
        Vector2D Bearing = Vector2D::Zero;
        unsigned int Advance = 0;
    };

    class IFont
    {
    public:
        virtual ~IFont() = default;

        virtual bool Load(const std::string& fontPath, unsigned int pixelHeight) = 0;
        virtual void Unload() = 0;

        virtual const std::map<char, CharacterInfo>& GetCharacters() const = 0;

        virtual void BindCharacterTexture(char c, unsigned int slot = 0) const {}
    };

}