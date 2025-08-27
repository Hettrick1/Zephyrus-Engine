#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Font.h"
#include "ShaderProgram.h"
#include "Shader.h"

#include <map>
#include <string>

/**
 * @brief Manages loading, storing, and retrieving textures, meshes, and fonts for the engine.
 * Provides static methods to access and clear loaded assets.
 */
class Assets
{
private:
	Assets() = default;

	// Loads a texture from file (internal use)
	static Texture LoadTextureFromFile(IRenderer& pRenderer, const std::string& pFilePath);
	// Loads a mesh from file (internal use)
	static Mesh* LoadMeshFromFile(const std::string& pFilePath);
	// Loads a font from file (internal use)
	static Font LoadFontFromFile(const std::string& pFilePath);
	static Shader LoadShaderFromFile(const std::string& pFilePath, ShaderType pType);

public:
	static std::map<std::string, Texture> mTextures;
	static std::map<std::string, Mesh*> mMeshes;
	static std::map<std::string, Font> mFonts;
	static std::map<std::string, Shader> mShaders;
	static std::map<std::string, ShaderProgram> mShaderPrograms;

	static const std::string IMPORT_PATH;
	static const std::string MESH_PATH;
	static const std::string FONT_PATH;
	static const std::string SHADER_PATH;

	// Loads a texture from file and stores it with the given name
	static Texture* LoadTexture(const std::string& pFilePath, const std::string& pName);
	static Texture& GetTexture(const std::string& pName);

	// Loads a mesh from file and stores it with the given name
	static Mesh* LoadMesh(const std::string& pFilePath, const std::string& pName);
	static Mesh* GetMesh(const std::string& pName);

	// Loads a font from file and stores it with the given name
	static Font* LoadFont(const std::string& pFilePath, const std::string& pName);
	static Font& GetFont(const std::string& pName);

	static Shader* LoadShader(const std::string& pFilePath, ShaderType pType, const std::string& pName);
	static Shader& GetShader(const std::string& pName);

	static ShaderProgram* LoadShaderProgram(std::vector<Shader*> pShaders, const std::string& pName);

	// Clears all loaded assets
	static void Clear();
};
