#pragma once

#include "glew.h"
#include "Vector2D.h"
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID = 0;            // ID handle of the glyph texture
    Vector2D Size = Vector2D::Zero;        // Size of glyph
    Vector2D Bearing = Vector2D::Zero; ;   // Offset from baseline to left/top of glyph
    unsigned int Advance = 0;              // Horizontal offset to advance to next glyph
};

class Font
{
public:
	Font();
	~Font();

    bool Load(std::string pFileName);

    inline std::map<GLchar, Character>& GetCharacters() { return mCharacters; }

private:
    std::map<GLchar, Character> mCharacters;
};
