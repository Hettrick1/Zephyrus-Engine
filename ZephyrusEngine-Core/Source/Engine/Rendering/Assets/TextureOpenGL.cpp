#include "TextureOpenGL.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Log.h"

Zephyrus::Assets::TextureOpenGL::~TextureOpenGL()
{
	glDeleteTextures(1, &mTextureID);
}

bool Zephyrus::Assets::TextureOpenGL::Load(const std::string& pFilename)
{
	mFilePath = pFilename;
	SDL_Surface* surface = IMG_Load(mFilePath.c_str());
	if (!surface)
	{
		ZP_CORE_ERROR("Failed to load texture file :" + mFilePath);
		return false;
	}
	mWidth = surface->w;
	mHeight = surface->h;

	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(surface);

	mWidth = converted->w;
	mHeight = converted->h;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
	SDL_FreeSurface(converted);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (mTextureID == 0)
	{
		ZP_INFO("Texture load failed");
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
