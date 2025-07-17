#pragma once

#include "Texture.h"
#include "Mesh.h"
#include <map>
#include <string>
#include "Font.h"

/**
 * @brief Manages loading, storing, and retrieving textures, meshes, and fonts for the engine.
 * Provides static methods to access and clear loaded assets.
 */
class Assets
{
public:
	static std::map<std::string, Texture> mTextures;
	static std::map<std::string, Mesh*> mMeshes;
	static std::map<std::string, Font> mFonts;

	// Loads a texture from file and stores it with the given name
	static Texture* LoadTexture(IRenderer& pRenderer, const std::string& pFilePath, const std::string& pName);

	static Texture& GetTexture(const std::string& pName);

	// Loads a mesh from file and stores it with the given name
	static Mesh* LoadMesh(const std::string& pFilePath, const std::string& pName);

	static Mesh* GetMesh(const std::string& pName);

	// Loads a font from file and stores it with the given name
	static Font* LoadFont(const std::string& pFilePath, const std::string& pName);

	static Font& GetFont(const std::string& pName);

	// Clears all loaded assets
	static void Clear();
private:
	Assets() = default;

	// Loads a texture from file (internal use)
	static Texture LoadTextureFromFile(IRenderer& pRenderer, const std::string& pFilePath);

	// Loads a mesh from file (internal use)
	static Mesh* LoadMeshFromFile(const std::string& pFilePath);

	// Loads a font from file (internal use)
	static Font LoadFontFromFile(const std::string& pFilePath);

};
