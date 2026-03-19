#pragma once

#include "Interface/ITexture2D.h"
#include "glew.h"
#include "GLFW/glfw3.h"

namespace Zephyrus::Assets
{
	class TextureGlfw
	{
	private:
		unsigned int mTextureID{ 0 };
		int mWidth{ 0 };
		int mHeight{ 0 };
		unsigned char* mData{nullptr};
		std::string mFilePath{""};
	public:
		TextureGlfw() = default;
		~TextureGlfw();
		
		bool Load(const std::string& pFilename);

		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }

		unsigned char* GetDatas() const { return mData; }
	};
}
