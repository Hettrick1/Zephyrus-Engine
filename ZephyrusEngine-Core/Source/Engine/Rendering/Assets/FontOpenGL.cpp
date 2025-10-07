#include "FontOpenGL.h"

#include "FontOpenGL.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Log.h"

namespace Zephyrus::Assets {
	FontOpenGL::~FontOpenGL()
	{
		Unload();
	}

	bool FontOpenGL::Load(const std::string& fontPath, unsigned int pixelHeight)
	{
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            ZP_CORE_ERROR("FREETYPE: Could not init FreeType Library");
            return false;
        }

        std::string font_name = fontPath;
        if (font_name.empty())
        {
            ZP_CORE_ERROR("FREETYPE: Failed to load font_name");
            return false;
        }

        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face))
        {
            ZP_CORE_ERROR("FREETYPE: Failed to load font");
            return false;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, pixelHeight);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    ZP_CORE_ERROR("FREETYPE: Failed to load GLYPHS");
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                CharacterGL ch;
                ch.TextureID = texture;
                ch.Size = Vector2D(face->glyph->bitmap.width, face->glyph->bitmap.rows);
                ch.Bearing = Vector2D(face->glyph->bitmap_left, face->glyph->bitmap_top);
                ch.Advance = face->glyph->advance.x;

                mCharacters[c] = ch;
                mCharactersInfos[c] = ch;
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return true;
	}

	void FontOpenGL::Unload()
	{
        for (auto& charTex : mCharacters)
        {
            glDeleteTextures(1, &charTex.second.TextureID);
        }
        mCharacters.clear();
	}

    const std::map<char, CharacterInfo>& FontOpenGL::GetCharacters() const
    {
        return mCharactersInfos;
    }

	void FontOpenGL::BindCharacterTexture(char c, unsigned int slot) const
	{
        auto it = mCharacters.find(c);
        if (it != mCharacters.end())
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, it->second.TextureID);
        }
	}
}
