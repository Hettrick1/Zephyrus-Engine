#pragma once
#include "Texture.h"

// Represents an OpenGL cube texture map, typically used for environment mapping (e.g., skyboxes).
class CubeTextureMap
{
public:

	// Loads and creates a cube texture map from the provided file paths for each face.
	bool CreateCubeTextureMap(const std::vector<std::string>& cube_face_paths);
	inline unsigned int GetID() const { return mTextureId; }
	void SetActive() const;
private:
	int mWidth, mHeight;
	unsigned int mTextureId;
	std::vector<std::string> mCubeFacePaths;
};