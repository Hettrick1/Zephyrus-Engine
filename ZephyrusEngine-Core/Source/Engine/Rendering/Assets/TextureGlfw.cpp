#include "pch.h"
#include "TextureGlfw.h"

#include "stb_image.h"
#include "Log.h"

Zephyrus::Assets::TextureGlfw::~TextureGlfw()
{
    stbi_image_free(mData);
}

bool Zephyrus::Assets::TextureGlfw::Load(const std::string& pFilename)
{
    mFilePath = pFilename;

    int width, height, channels;

    //stbi_set_flip_vertically_on_load(true);

    unsigned  char* data = stbi_load(pFilename.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        ZP_CORE_ERROR("Failed to load texture file: " + pFilename);
        return false;
    }
	
    mWidth = width;
    mHeight = height;
    mData = data;

    return true;
}
