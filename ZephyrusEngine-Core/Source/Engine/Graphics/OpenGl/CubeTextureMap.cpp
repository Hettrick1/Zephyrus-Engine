#include "CubeTextureMap.h"
#include "RendererOpenGl.h"

bool CubeTextureMap::CreateCubeTextureMap(const std::vector<std::string>& pCubePaths)
{
	unsigned char* data = nullptr;
	int width = 0, height = 0, channels = 0;

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);

	mCubeFacePaths = pCubePaths;

	for (unsigned int i = 0; i < pCubePaths.size(); i++) // retrieve the skybox textures
	{
		SDL_Surface* surface = IMG_Load(mCubeFacePaths[i].c_str());
		if (!surface)
		{
			Log::Info("Failed to load texture file :" + mCubeFacePaths[i]);
			return false;
		}
		mWidth = surface->w;
		mHeight = surface->h;

		int format = 0;
		if (surface->format->format == SDL_PIXELFORMAT_RGB24)
		{
			format = GL_RGB;
		}
		else if (surface->format->format == SDL_PIXELFORMAT_RGBA32)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
		SDL_FreeSurface(surface);

		Log::Info("Loaded GL texture : " + mCubeFacePaths[i]);
	}

	// Setting some image parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void CubeTextureMap::SetActive() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
}
