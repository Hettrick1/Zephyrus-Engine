#pragma once
#include "Interface/ICubeMapTexture.h"

namespace Zephyrus::Assets {
	// Represents an OpenGL cube texture map, typically used for environment mapping (e.g., skyboxes).
	class CubemapTextureOpenGL : public ICubeMapTexture
	{
	private:
		int mWidth{ 0 };
		int mHeight{ 0 };
		unsigned int mTextureId{ 0 };
		std::vector<std::string> mCubeFaceIds;
		std::vector<std::string> mTempFaceIds;
		TextureType mType{TextureType::Cubemap};
	public:
		CubemapTextureOpenGL() = default;
		// Loads and creates a cube texture map from the provided file paths for each face.
		bool Load(const std::vector<std::string>& pCubeIds) override;
		inline uint64_t GetHandle() const override { return mTextureId; }
		void Bind(unsigned int slot = 0) const override;
		void Unbind(unsigned int slot = 0) const override;

		int GetWidth() const override { return mWidth; }
		int GetHeight() const override { return mHeight; }

		std::vector<std::string> GetFaceFileIds() const override { return mCubeFaceIds; }
		void SetTempFileIds(const std::vector<std::string>& pFileIds) override { mTempFaceIds = pFileIds; }
		std::vector<std::string> GetTempFileIds() const override { return mTempFaceIds; }

		TextureType GetType() const override { return mType; }
		void SetType(TextureType newType) override { mType = newType;}
	};
}