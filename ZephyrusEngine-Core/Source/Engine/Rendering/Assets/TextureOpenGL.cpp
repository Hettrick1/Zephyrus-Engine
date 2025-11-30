#include "pch.h"
#include "TextureOpenGL.h"
#include "stb_image.h"
#include "Log.h"

Zephyrus::Assets::TextureOpenGL::~TextureOpenGL()
{
	glDeleteTextures(1, &mTextureID);
}

bool Zephyrus::Assets::TextureOpenGL::Load(const std::string& pFilename)
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

	GLenum format = GL_RGB;
	GLenum internalFormat = GL_RGB;

	if (channels == 4) { format = GL_RGBA; internalFormat = GL_RGBA; }
	else if (channels == 3) { format = GL_RGB; internalFormat = GL_RGB; }
	else if (channels == 1) { format = GL_RED; internalFormat = GL_RED; }

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
	
	if (mTextureID == 0)
	{
		ZP_INFO("Texture load failed");
		return false;
	}

	return true;
}

void Zephyrus::Assets::TextureOpenGL::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Zephyrus::Assets::TextureOpenGL::Unbind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
