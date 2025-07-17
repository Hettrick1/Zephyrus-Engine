#include "Texture.h"

#include "RendererSdl.h"
#include "RendererOpenGl.h"

Texture::Texture()
	: mHeight(0), mWidth(0), mSdlTexture(nullptr)
{

}

Texture::~Texture()
{
}

bool Texture::Load(IRenderer& pRenderer, const std::string& pFilename)
{
	mFilePath = pFilename;
	SDL_Surface* surface = IMG_Load(mFilePath.c_str());
	if (!surface) 
	{
		Log::Error(LogType::Application, "Failed to load texture file :" + mFilePath);
		return false;
	}
	mWidth = surface->w;
	mHeight = surface->h;

	if (pRenderer.GetType() == IRenderer::RendererType::SDL)
	{
		return LoadSdl(static_cast<RendererSdl*>(&pRenderer), pFilename, surface);
	}
	return LoadGl(static_cast<RendererOpenGl*>(&pRenderer), pFilename, surface);
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

Vector3D Texture::GetTextureSize()
{
	return {static_cast<float>(mWidth), static_cast<float>(mHeight), 0};
}

bool Texture::LoadSdl(RendererSdl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface)
{
	mSdlTexture = SDL_CreateTextureFromSurface(pRenderer->ToSdlRenderer(), pSurface);
	SDL_FreeSurface(pSurface);
	if (!mSdlTexture) { 
		Log::Error(LogType::Render, "Failed to convert surface to texture :" + mFilePath); 
		return false;
	}
	Log::Info("Loaded texture : " + mFilePath);
	return true;
}

bool Texture::LoadGl(RendererOpenGl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface)
{
	int format = 0;
	if (pSurface->format->format == SDL_PIXELFORMAT_RGB24)
	{
		format = GL_RGB;
	}
	else if (pSurface->format->format == SDL_PIXELFORMAT_RGBA32)
	{
		format = GL_RGBA;
	}
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);
	SDL_FreeSurface(pSurface);

	Log::Info("Loaded GL texture : " + mFilePath);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return true;
}
