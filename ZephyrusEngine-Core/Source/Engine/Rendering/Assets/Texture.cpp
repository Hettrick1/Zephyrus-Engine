#include "Texture.h"

#include "RendererOpenGl.h"

namespace Zephyrus::Assets {
	Texture::Texture()
		: mHeight(0), mWidth(0), mSdlTexture(nullptr)
	{

	}

	Texture::~Texture()
	{
	}

	bool Texture::Load(Zephyrus::Render::IRenderer& pRenderer, const std::string& pFilename)
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

		return LoadGl(static_cast<Zephyrus::Render::RendererOpenGl*>(&pRenderer), pFilename, surface);
	}

	void Texture::Unload()
	{
		if (mSdlTexture)
		{
			SDL_DestroyTexture(mSdlTexture);
		}
		else
		{
			glDeleteTextures(1, &mTextureId);
		}
	}

	void Texture::SetActive() const
	{
		glBindTexture(GL_TEXTURE_2D, mTextureId);
	}

	void Texture::UpdateInfo(int& width, int& height)
	{
		width = mWidth;
		height = mHeight;
	}

	void Texture::OverrideTextureSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	bool Texture::LoadGl(Zephyrus::Render::RendererOpenGl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface)
	{
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(pSurface, SDL_PIXELFORMAT_ABGR8888, 0);
		SDL_FreeSurface(pSurface);

		mWidth = converted->w;
		mHeight = converted->h;

		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
		SDL_FreeSurface(converted);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (mTextureId == 0)
		{
			ZP_INFO("Texture load failed");
		}

		return true;
	}
}