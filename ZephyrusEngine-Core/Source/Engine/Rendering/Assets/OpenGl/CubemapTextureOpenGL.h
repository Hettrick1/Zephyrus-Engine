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
		std::vector<std::string> mCubeFacePaths;
		std::vector<std::string> mTempFacePaths;
		TextureType mType{TextureType::Cubemap};
	public:
		CubemapTextureOpenGL() = default;
		// Loads and creates a cube texture map from the provided file paths for each face.
		bool Load(const std::vector<std::string>& pCubePaths) override;
		inline uint64_t GetHandle() const override { return mTextureId; }
		void Bind(unsigned int slot = 0) const override;
		void Unbind(unsigned int slot = 0) const override;

		int GetWidth() const override { return mWidth; }
		int GetHeight() const override { return mHeight; }

		std::vector<std::string> GetFaceFilePath() const override { return mCubeFacePaths; }
		void SetTempFilePath(const std::vector<std::string>& pFilePath) override { mTempFacePaths = pFilePath; }
		std::vector<std::string> GetTempFilePath() const override { return mTempFacePaths; }

		TextureType GetType() const override { return mType; }
		void SetType(TextureType newType) override { mType = newType;}
	};
}