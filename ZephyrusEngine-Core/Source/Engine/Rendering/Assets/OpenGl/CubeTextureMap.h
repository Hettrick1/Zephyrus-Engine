#pragma once
#include <vector>
#include <string>

namespace Zephyrus::Assets {
	// Represents an OpenGL cube texture map, typically used for environment mapping (e.g., skyboxes).
	class CubeTextureMap
	{
	private:
		int mWidth, mHeight;
		unsigned int mTextureId;
		std::vector<std::string> mCubeFacePaths;
		std::vector<std::string> mTempFacePaths;
	public:
		// Loads and creates a cube texture map from the provided file paths for each face.
		bool CreateCubeTextureMap(const std::vector<std::string>& pCubePaths);
		inline unsigned int GetID() const { return mTextureId; }
		void SetActive() const;
		inline std::vector<std::string> GetFaceFilePath() const { return mCubeFacePaths; }
		inline void SetTempFilePath(const std::vector<std::string>& pFilePath) { mTempFacePaths = pFilePath; }
		inline std::vector<std::string> GetTempFilePath() const { return mTempFacePaths; }
	};
}