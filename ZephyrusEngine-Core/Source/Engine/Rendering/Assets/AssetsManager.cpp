#include "AssetsManager.h"
#include <sstream>
#include "Log.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Scene.h"
#include "ISceneContext.h"
#include "Data/MeshData.h"
#include "Interface/IMesh.h"
#include "Interface/IFont.h"
#include "Interface/ITexture2D.h"
#include "Material/IMaterial.h"
#include "ISerializationFactory.h"
#include <filesystem>

namespace Zephyrus::Assets {
	std::map<std::string, ITexture2D*> AssetsManager::mTextures = {};
	std::map<std::string, IFont*> AssetsManager::mFonts = {};
	std::map<std::string, IMesh*> AssetsManager::mMeshes = {};
	std::map<std::string, Render::IShader*> AssetsManager::mShaders = {};
	std::map<std::string, Render::IShaderProgram*> AssetsManager::mShaderPrograms = {};
	std::map<std::string, ICubeMapTexture*> AssetsManager::mCubemaps = {};
	std::map<std::string, Material::IMaterial*> AssetsManager::mMaterials = {};

	ISceneContext* AssetsManager::mContext{ nullptr };

	const std::string AssetsManager::IMPORT_PATH = "../Content/";
	const std::string AssetsManager::MESH_PATH = "../Content/Meshes/";
	const std::string AssetsManager::FONT_PATH = "../Content/Fonts/";
	const std::string AssetsManager::SHADER_PATH = "../Content/Shaders/";

	ITexture2D* AssetsManager::LoadTexture(const std::string& pFilePath, const std::string& pName)
	{
		if (mTextures.find(pName) == mTextures.end()) {
			mTextures[pName] = LoadTextureFromFile(GetFullPath(pFilePath, AssetType::Texture));
			return mTextures[pName];
		}
		return mTextures[pName];
	}

	ITexture2D* AssetsManager::GetTexture(const std::string& pName)
	{
		if (mTextures.find(pName) == mTextures.end()) {
			std::ostringstream loadError;
			loadError << "Texture " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mTextures[pName];
	}

	ICubeMapTexture* AssetsManager::LoadCubemap(const std::vector<std::string>& pCubePaths, const std::string& pName)
	{
		if (mCubemaps.find(pName) == mCubemaps.end()) {
			mCubemaps[pName] = LoadCubemapFromFile(pCubePaths);
			return mCubemaps[pName];
		}
		return mCubemaps[pName];
	}

	ICubeMapTexture* AssetsManager::GetCubemap(const std::string& pName)
	{
		if (mCubemaps.find(pName) == mCubemaps.end()) {
			std::ostringstream loadError;
			loadError << "Cubemap " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mCubemaps[pName];
	}

	void AssetsManager::SetContext(ISceneContext* pContext)
	{
		mContext = pContext;
	}

	IMesh* AssetsManager::LoadMesh(const std::string& pFilePath, const std::string& pName)
	{
		if (mMeshes.find(pName) == mMeshes.end()) {
			auto data = LoadMeshData(GetFullPath(pFilePath, AssetType::Mesh));
			mMeshes[pName] = mContext->GetRenderer()->LoadMeshFromData(data);
			return mMeshes[pName];
		}
		return mMeshes[pName];
	}

	IMesh* AssetsManager::GetMesh(const std::string& pName)
	{
		if (mMeshes.find(pName) == mMeshes.end()) {
			std::ostringstream loadError;
			loadError << "Mesh " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mMeshes[pName];
	}

	IFont* AssetsManager::LoadFont(const std::string& pFilePath, const std::string& pName)
	{
		if (mFonts.find(pName) == mFonts.end()) {
			mFonts[pName] = LoadFontFromFile(GetFullPath(pFilePath, AssetType::Font));
			return mFonts[pName];
		}
		return mFonts[pName];
	}

	IFont* AssetsManager::GetFont(const std::string& pName)
	{
		if (mFonts.find(pName) == mFonts.end()) {
			std::ostringstream loadError;
			loadError << "Font " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mFonts[pName];
	}

	Render::IShader* AssetsManager::LoadShader(const std::string& pFilePath, Render::ShaderType pType, const std::string& pName)
	{
		if (mShaders.find(pName) == mShaders.end()) {
			mShaders[pName] = LoadShaderFromFile(GetFullPath(pFilePath, AssetType::Shader), pType);
			return mShaders[pName];
		}
		return mShaders[pName];
	}

	Render::IShader* AssetsManager::GetShader(const std::string& pName)
	{
		if (mShaders.find(pName) == mShaders.end()) {
			std::ostringstream loadError;
			loadError << "Shader " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
		}
		return mShaders[pName];
	}

	Render::IShaderProgram* AssetsManager::LoadShaderProgram(std::vector<Render::IShader*> pShaders, const std::string& pName)
	{
		if (mShaderPrograms.find(pName) == mShaderPrograms.end()) {
			mShaderPrograms[pName] = LoadProgramWithShaders(pShaders);
			return mShaderPrograms[pName];
		}
		return mShaderPrograms[pName];
	}

	Material::IMaterial* AssetsManager::LoadMaterial(const std::string& pFilePath, const std::string& pName)
	{
		if (mMaterials.find(pName) == mMaterials.end()) {
			mMaterials[pName] = LoadMaterialFromFile(pFilePath);
			return mMaterials[pName];
		}
		return mMaterials[pName];
	}

	Material::IMaterial* AssetsManager::GetMaterial(const std::string& pName)
	{
		if (mMaterials.find(pName) == mMaterials.end()) {
			std::ostringstream loadError;
			loadError << "Shader " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
		}
		return mMaterials[pName];
	}

	void AssetsManager::Clear()
	{
		for (auto& iter : mTextures)
		{
			if (iter.second)
			{
				delete iter.second;
				iter.second = nullptr;
			}
		}
		mTextures.clear();
		for (auto& iter : mMeshes)
		{
			if (iter.second)
			{
				delete iter.second;
				iter.second = nullptr;
			}
		}
		mMeshes.clear();
		for (auto& iter : mFonts)
		{
			if (iter.second)
			{
				iter.second->Unload();
				delete iter.second;
				iter.second = nullptr;
			}
		}
		mFonts.clear();
		for (auto& iter : mShaders)
		{
			iter.second->Unload();
			delete iter.second;
			iter.second = nullptr;
		}
		mShaders.clear();
		for (auto& iter : mShaderPrograms)
		{
			iter.second->Unload();
			delete iter.second;
			iter.second = nullptr;
		}
		mShaderPrograms.clear();
		for (auto& iter : mMaterials)
		{
			delete iter.second;
			iter.second = nullptr;
		}
		mMaterials.clear();
	}

	ITexture2D* AssetsManager::LoadTextureFromFile(const std::string& pFilePath)
	{
		return mContext->GetRenderer()->LoadTexture(pFilePath);
	}

	MeshData AssetsManager::LoadMeshData(const std::string& pFilePath)
	{
		MeshData data;
		data.sourceFile = pFilePath;

		std::string extension = std::filesystem::path(pFilePath).extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".obj")
		{
			tinyobj::attrib_t attributes;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warning, errors;

			bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &errors, pFilePath.c_str());
			if (!success)
			{
				ZP_CORE_ERROR("Mesh " + pFilePath + " does not exist or is not .obj");
				return data;
			}

			std::vector<Render::Vertex> vertices;
			std::vector<uint32_t> indices;
			for (auto& shape : shapes)
			{
				auto& mesh = shape.mesh;
				for (auto& index : mesh.indices)
				{
					Vector3D position = {
						attributes.vertices[index.vertex_index * 3],
						attributes.vertices[index.vertex_index * 3 + 1],
						attributes.vertices[index.vertex_index * 3 + 2]
					};
					Vector3D normal = {
						attributes.normals[index.normal_index * 3],
						attributes.normals[index.normal_index * 3 + 1],
						attributes.normals[index.normal_index * 3 + 2]
					};
					Vector2D texCoord = {
						attributes.texcoords[index.texcoord_index * 2],
						attributes.texcoords[index.texcoord_index * 2 + 1]
					};
					vertices.push_back({ position, normal, texCoord });
					indices.push_back(static_cast<uint32_t>(indices.size()));
				}
			}

			data.vertices = std::move(vertices);
			data.indices = std::move(indices);
			ZP_LOAD("Mesh " + pFilePath + " successfully loaded");
		}
		else if (extension == ".fbx")
		{
			ZP_CORE_WARN("FBX loading not implemented yet.");
		}
		else
		{
			ZP_CORE_ERROR("Unsupported mesh format: " + extension);
		}

		return data;
	}

	IFont* AssetsManager::LoadFontFromFile(const std::string& pFilePath)
	{
		return mContext->GetRenderer()->LoadFont(pFilePath);
	}

	Render::IShader* AssetsManager::LoadShaderFromFile(const std::string& pFilePath, ShaderType pType)
	{
		return mContext->GetRenderer()->LoadShader(pFilePath, pType);
	}

	Render::IShaderProgram* AssetsManager::LoadProgramWithShaders(std::vector<Render::IShader*> pShaders)
	{
		return mContext->GetRenderer()->LoadShaderProgram(pShaders);
	}

	ICubeMapTexture* AssetsManager::LoadCubemapFromFile(const std::vector<std::string>& pCubePaths)
	{
		return mContext->GetRenderer()->LoadCubemap(pCubePaths);
	}

	Material::IMaterial* AssetsManager::LoadMaterialFromFile(const std::string& pFilePath)
	{
		auto mat = mContext->GetRenderer()->CreateMaterial();
		mat->SetFilePath(pFilePath);
		auto reader = mContext->GetSerializationFactory()->CreateDeserializer();
		reader->LoadDocument(pFilePath);
		if (reader->LoadDocument(pFilePath))
		{
			mat->Deserialize(*reader);
		}
		else
		{
			return nullptr;
		}
		return mat;
	}

	std::string AssetsManager::GetFullPath(const std::string& pPath, AssetType pType)
	{
		std::string newPath;
		switch (pType)
		{
		case AssetType::Texture:
		{
			if (pPath.find(IMPORT_PATH) == std::string::npos)
			{
				newPath = IMPORT_PATH + pPath;
				break;
			}
			newPath = pPath;
			break;
		}
		case AssetType::Mesh:
		{
			if (pPath.find(MESH_PATH) == std::string::npos)
			{
				newPath = MESH_PATH + pPath;
				break;
			}
			newPath = pPath;
			break;
		}
		case AssetType::Font:
		{
			if (pPath.find(FONT_PATH) == std::string::npos)
			{
				newPath = FONT_PATH + pPath;
				break;
			}
			newPath = pPath;
			break;
		}
		case AssetType::Shader:
		{
			if (pPath.find(SHADER_PATH) == std::string::npos)
			{
				newPath = SHADER_PATH + pPath;
				break;
			}
			newPath = pPath;
			break;
		}
		default:
		{
			newPath = pPath;
			break;
		}
		}
		return newPath;
	}
}
