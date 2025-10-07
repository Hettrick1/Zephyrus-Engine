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

class ISceneContext;

using Zephyrus::Render::ShaderProgram;
using Zephyrus::Render::Shader;
using Zephyrus::Render::ShaderType;

namespace Zephyrus::Assets {

	class IMesh;
	struct MeshData;
	class ITexture;
	class IFont;

	enum class AssetType
	{
		Mesh,
		Texture,
		Font,
		Shader
	};

	Zephyrus::Render::Vertex;

	class AssetsManager
	{
	private:
		AssetsManager() = default;

		// Loads a texture from file (internal use)
		static ITexture* LoadTextureFromFile(const std::string& pFilePath);
		// Loads a mesh from file (internal use)
		static Mesh* LoadMeshFromFile(const std::string& pFilePath);

		static MeshData LoadMeshData(const std::string& pFilePath);

		// Loads a font from file (internal use)
		static IFont* LoadFontFromFile(const std::string& pFilePath);
		static Shader LoadShaderFromFile(const std::string& pFilePath, ShaderType pType);

	public:
		static std::map<std::string, ITexture*> mTextures;
		static std::map<std::string, IMesh*> mMeshes;
		static std::map<std::string, IFont*> mFonts;
		static std::map<std::string, Shader> mShaders;
		static std::map<std::string, ShaderProgram> mShaderPrograms;
		static ISceneContext* mContext;

		static const std::string IMPORT_PATH;
		static const std::string MESH_PATH;
		static const std::string FONT_PATH;
		static const std::string SHADER_PATH;

		// Loads a texture from file and stores it with the given name
		static Zephyrus::Assets::ITexture* LoadTexture(const std::string& pFilePath, const std::string& pName);
		static Zephyrus::Assets::ITexture* GetTexture(const std::string& pName);

		static void SetContext(ISceneContext* pContext);

		// Loads a mesh from file and stores it with the given name
		static IMesh* LoadMesh(const std::string& pFilePath, const std::string& pName);
		static IMesh* GetMesh(const std::string& pName);

		// Loads a font from file and stores it with the given name
		static IFont* LoadFont(const std::string& pFilePath, const std::string& pName);
		static IFont* GetFont(const std::string& pName);

		static Shader* LoadShader(const std::string& pFilePath, ShaderType pType, const std::string& pName);
		static Shader& GetShader(const std::string& pName);

		static ShaderProgram* LoadShaderProgram(std::vector<Shader*> pShaders, const std::string& pName);

		static std::string GetFullPath(const std::string& pPath, AssetType pType);

		// Clears all loaded assets
		static void Clear();
	};
}
