#pragma once

#include "Interface/ITexture2D.h"
#include "glew.h"

namespace Zephyrus::Assets
{
	class TextureOpenGL : public ITexture2D
	{
	private:
		unsigned int mTextureID{ 0 };
		int mWidth{ 0 };
		int mHeight{ 0 };
		std::string mFilePath{""};
		TextureType mType{TextureType::Texture2D};
	public:
		TextureOpenGL() = default;
		~TextureOpenGL() override;
		
		bool Load(const std::string& pFilename) override;

		int GetWidth() const override { return mWidth; }
		int GetHeight() const override { return mHeight; }

		void Bind(unsigned int slot = 0) const override;

		void Unbind(unsigned int slot = 0) const override;

		uint64_t GetHandle() const override { return mTextureID; }

		std::string GetFilePath() const override { return mFilePath; }
		TextureType GetType() const override { return mType; }
		void SetType(TextureType newType) override { mType = newType;}
	};
}
