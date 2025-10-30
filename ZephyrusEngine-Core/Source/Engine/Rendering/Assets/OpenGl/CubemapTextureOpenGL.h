#pragma once
#include "Interface/ICubeMapTexture.h"

namespace Zephyrus::Assets {
	// Represents an OpenGL cube texture map, typically used for environment mapping (e.g., skyboxes).
	class CubeTextureMap : public ICubeMapTexture
	{
	private:
		int mWidth{ 0 };
		int mHeight{ 0 };
		unsigned int mTextureId{ 0 };
		std::vector<std::string> mCubeFacePaths;
		std::vector<std::string> mTempFacePaths;
	public:
		CubeTextureMap() = default;
		// Loads and creates a cube texture map from the provided file paths for each face.
		bool Load(const std::vector<std::string>& pCubePaths) override;
		inline uint64_t GetHandle() const override { return mTextureId; }
		void SetActive() const override;
		inline std::vector<std::string> GetFaceFilePath() const override { return mCubeFacePaths; }
		inline void SetTempFilePath(const std::vector<std::string>& pFilePath) override { mTempFacePaths = pFilePath; }
		inline std::vector<std::string> GetTempFilePath() const override { return mTempFacePaths; }
	};
}