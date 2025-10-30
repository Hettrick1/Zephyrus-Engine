#pragma once

#include "../Interface/IShaderProgram.h"
#include "../Interface/IShader.h"
#include "Vertex.h"

#include <map>
#include <string>

/**
 * @brief Manages loading, storing, and retrieving textures, meshes, and fonts for the engine.
 * Provides static methods to access and clear loaded assets.
 */

class ISceneContext;

namespace Zephyrus::Assets {

	class IMesh;
	struct MeshData;
	class ITexture;
	class IFont;
	class ICubeMapTexture;

	enum class AssetType
	{
		Mesh,
		Texture,
		Font,
		Shader
	};

	class AssetsManager
	{
	private:
		AssetsManager() = default;

		// Loads a texture from file (internal use)
		static ITexture* LoadTextureFromFile(const std::string& pFilePath);

		static MeshData LoadMeshData(const std::string& pFilePath);

		// Loads a font from file (internal use)
		static IFont* LoadFontFromFile(const std::string& pFilePath);
		static Render::IShader* LoadShaderFromFile(const std::string& pFilePath, Render::ShaderType pType);
		static Render::IShaderProgram* LoadProgramWithShaders(std::vector<Render::IShader*> pShaders);
		static ICubeMapTexture* LoadCubemapFromFile(const std::vector<std::string>& pCubePaths);

	public:
		static std::map<std::string, ITexture*> mTextures;
		static std::map<std::string, IMesh*> mMeshes;
		static std::map<std::string, IFont*> mFonts;
		static std::map<std::string, Render::IShader*> mShaders;
		static std::map<std::string, Render::IShaderProgram*> mShaderPrograms;
		static std::map<std::string, ICubeMapTexture*> mCubemaps;
		static ISceneContext* mContext;

		static const std::string IMPORT_PATH;
		static const std::string MESH_PATH;
		static const std::string FONT_PATH;
		static const std::string SHADER_PATH;

		static void SetContext(ISceneContext* pContext);

		// Loads a texture from file and stores it with the given name
		static ITexture* LoadTexture(const std::string& pFilePath, const std::string& pName);
		static ITexture* GetTexture(const std::string& pName);

		static ICubeMapTexture* LoadCubemap(const std::vector<std::string>& pCubePaths, const std::string& pName);
		static ICubeMapTexture* GetCubemap(const std::string& pName);

		// Loads a mesh from file and stores it with the given name
		static IMesh* LoadMesh(const std::string& pFilePath, const std::string& pName);
		static IMesh* GetMesh(const std::string& pName);

		// Loads a font from file and stores it with the given name
		static IFont* LoadFont(const std::string& pFilePath, const std::string& pName);
		static IFont* GetFont(const std::string& pName);

		static Render::IShader* LoadShader(const std::string& pFilePath, Render::ShaderType pType, const std::string& pName);
		static Render::IShader* GetShader(const std::string& pName);

		static Render::IShaderProgram* LoadShaderProgram(std::vector<Render::IShader*> pShaders, const std::string& pName);

		static std::string GetFullPath(const std::string& pPath, AssetType pType);

		// Clears all loaded assets
		static void Clear();
	};
}
