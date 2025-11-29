#include "CubemapTextureOpenGL.h"
#include "stb_image.h"
#include "glew.h"
#include "AssetsManager.h"
#include "Log.h"

namespace Zephyrus::Assets {
	bool CubemapTextureOpenGL::Load(const std::vector<std::string>& pCubePaths)
	{
		int width = 0, height = 0, channels = 0;

		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);

		mTempFacePaths = pCubePaths;
		mCubeFacePaths = pCubePaths;

		stbi_set_flip_vertically_on_load(false);

		for (unsigned int i = 0; i < pCubePaths.size(); i++) // retrieve the skybox textures
		{

			std::string path = AssetsManager::GetFullPath(mCubeFacePaths[i], AssetType::Texture);

			unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			if (!data)
			{
				ZP_CORE_ERROR("Failed to load cubemap face: " + mCubeFacePaths[i]);
				return false;
			}
			mWidth  = width;
			mHeight = height;

			GLenum format = GL_RGB;
			if (channels == 4) format = GL_RGBA;
			else if (channels == 3) format = GL_RGB;
			else if (channels == 1) format = GL_RED;
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			ZP_LOAD("Loaded GL texture : " + AssetsManager::IMPORT_PATH + mCubeFacePaths[i]);
		}

		// Setting some image parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return true;
	}

	void CubemapTextureOpenGL::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
	}

	void CubemapTextureOpenGL::Unbind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

}