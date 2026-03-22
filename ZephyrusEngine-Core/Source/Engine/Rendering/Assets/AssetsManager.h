#pragma once

#include "../Interface/IShaderProgram.h"
#include "../Interface/IShader.h"
#include "FileSystem/AssetDataBase.h"
#include "Vertex.h"

#include <map>
#include <string>

/**
 * @brief Manages loading, storing, and retrieving textures, meshes, and fonts for the engine.
 * Provides static methods to access and clear loaded assets.
 */

class ISceneContext;

namespace Zephyrus::Material
{
	class IMaterial;
}

namespace Zephyrus::Assets {

	class IMesh;
	struct MeshData;
	class ITexture2D;
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
		ITexture2D* LoadTextureFromFile(const std::string& pFilePath);
		MeshData LoadMeshData(const std::string& pId);
		IFont* LoadFontFromFile(const std::string& pFilePath);
		Render::IShader* LoadShaderFromFile(const std::string& pFilePath, Render::ShaderType pType);
		Render::IShaderProgram* LoadProgramWithShaders(std::vector<Render::IShader*> pShaders);
		ICubeMapTexture* LoadCubemapFromFile(const std::vector<std::string>& pCubePaths);
		Material::IMaterial* LoadMaterialFromFile(const std::string& pFilePath);

		std::map<std::string, ITexture2D*> mTextures = {};
		std::map<std::string, IMesh*> mMeshes = {};
		std::map<std::string, IFont*> mFonts = {};
		std::map<std::string, Render::IShader*> mShaders = {};
		std::map<std::string, Render::IShaderProgram*> mShaderPrograms = {};
		std::map<std::string, ICubeMapTexture*> mCubemaps = {};
		std::map<std::string, Material::IMaterial*> mMaterials = {};
		ISceneContext* mContext = {nullptr};

		AssetDataBase database;

	public:
		AssetsManager(AssetsManager const&) = delete;
		void operator=(AssetsManager const&) = delete;

		static AssetsManager& GetInstance();

		// TODO : Remove this, with ids we won't need them anymore
		const std::string IMPORT_PATH = "../Content/";
		const std::string MESH_PATH = "../Content/Meshes/";
		const std::string FONT_PATH = "../Content/Fonts/";
		const std::string SHADER_PATH= "../Content/Shaders/";
		const std::string PLACE_HOLDER_TEXTURE_PATH = "../Content/Sprites/uv_mapper.jpg";

		// Set the context and init asset database
		void Initialize(ISceneContext* pContext);

		// Loads a texture from file and stores it with the given name
		ITexture2D* LoadTexture(const std::string& pFilePath, const std::string& pName);
		ITexture2D* GetTexture(const std::string& pName);

		ICubeMapTexture* LoadCubemap(const std::vector<std::string>& pCubePaths, const std::string& pName);
		ICubeMapTexture* GetCubemap(const std::string& pName);

		// Loads a mesh from file and stores it with the given name
		IMesh* LoadMesh(const std::string& pId, bool pForceReload = false);
		IMesh* GetMesh(const std::string& pName);

		// Loads a font from file and stores it with the given name
		IFont* LoadFont(const std::string& pFilePath, const std::string& pName);
		IFont* GetFont(const std::string& pName);

		Render::IShader* LoadShader(const std::string& pFilePath, Render::ShaderType pType, const std::string& pName);
		Render::IShader* GetShader(const std::string& pName);

		Render::IShaderProgram* LoadShaderProgram(std::vector<Render::IShader*> pShaders, const std::string& pName);

		Material::IMaterial* LoadMaterial(const std::string& pFilePath, const std::string& pName);
		Material::IMaterial* GetMaterial(const std::string& pName);

		std::string GetFullPath(const std::string& pPath, AssetType pType);

		// Clears all loaded assets
		void CleanUp();
	};
}
