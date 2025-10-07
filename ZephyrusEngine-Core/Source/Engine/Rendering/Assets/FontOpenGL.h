#pragma once

#include "Interface/IFont.h"
#include "glew.h"

namespace Zephyrus::Assets {

    struct CharacterGL : CharacterInfo
    {
        unsigned int TextureID = 0;
    };

    class FontOpenGL : public IFont
    {
    private:
        std::map<char, CharacterGL> mCharacters;
        std::map<char, CharacterInfo> mCharactersInfos;
    public:
        ~FontOpenGL();

        bool Load(const std::string& fontPath, unsigned int pixelHeight = 128) override;
        void Unload() override;
        const std::map<char, CharacterInfo>& GetCharacters() const override;

        void BindCharacterTexture(char c, unsigned int slot = 0) const override;
    };

}