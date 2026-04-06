#include "pch.h"
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
	
	ITexture2D* AssetsManager::LoadTexture(const std::string& pId, bool pForceReload)
	{
		if (!mTextures.contains(pId) || pForceReload) {
			mTextures[pId] = LoadTextureFromFile(pId);
			return mTextures[pId];
		}
		return mTextures[pId];
	}

	ITexture2D* AssetsManager::GetTexture(const std::string& pId)
	{
		if (!mTextures.contains(pId)) {
			std::ostringstream loadError;
			loadError << "Texture " << pId << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mTextures[pId];
	}

	ICubeMapTexture* AssetsManager::LoadCubemap(const std::vector<std::string>& pCubeIds, const std::string& pName, bool pForceReload)
	{
		if (!mCubemaps.contains(pName) || pForceReload) {
			mCubemaps[pName] = LoadCubemapFromFile(pCubeIds);
			return mCubemaps[pName];
		}
		return mCubemaps[pName];
	}

	ICubeMapTexture* AssetsManager::GetCubemap(const std::string& pName)
	{
		if (!mCubemaps.contains(pName)) {
			std::ostringstream loadError;
			loadError << "Cubemap " << pName << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mCubemaps[pName];
	}

	AssetsManager& AssetsManager::GetInstance()
	{
		static AssetsManager instance;
		return instance;
	}

	void AssetsManager::Initialize(ISceneContext* pContext)
	{
		mContext = pContext;
		mFileDataBase.Init(mContext);
	}

	IMesh* AssetsManager::LoadMesh(const std::string& pId, bool pForceReload)
	{
		if (!mMeshes.contains(pId) || pForceReload) {
			auto data = LoadMeshData(pId);
			mMeshes[pId] = mContext->GetRenderer()->LoadMeshFromData(data);
			return mMeshes[pId];
		}
		return mMeshes[pId];
	}

	IMesh* AssetsManager::GetMesh(const std::string& pId)
	{
		if (!mMeshes.contains(pId)) {
			std::ostringstream loadError;
			loadError << "Mesh " << pId << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mMeshes[pId];
	}

	IFont* AssetsManager::LoadFont(const std::string& pId, bool pForceReload)
	{
		if (!mFonts.contains(pId) || pForceReload) {
			mFonts[pId] = LoadFontFromFile(pId);
			return mFonts[pId];
		}
		return mFonts[pId];
	}

	IFont* AssetsManager::GetFont(const std::string& pId)
	{
		if (!mFonts.contains(pId)) {
			std::ostringstream loadError;
			loadError << "Font " << pId << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
			return nullptr;
		}
		return mFonts[pId];
	}

	Render::IShader* AssetsManager::LoadShader(const std::string& pId, Render::ShaderType pType, bool pForceReload)
	{
		if (!mShaders.contains(pId) || pForceReload) {
			mShaders[pId] = LoadShaderFromFile(pId, pType);
			return mShaders[pId];
		}
		return mShaders[pId];
	}

	Render::IShader* AssetsManager::GetShader(const std::string& pId)
	{
		if (!mShaders.contains(pId)) {
			std::ostringstream loadError;
			loadError << "Shader " << pId << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
		}
		return mShaders[pId];
	}

	Render::IShaderProgram* AssetsManager::LoadShaderProgram(const std::vector<Render::IShader*>& pShaders, const std::string& pName, bool pForceReload)
	{
		if (!mShaderPrograms.contains(pName) || pForceReload) {
			mShaderPrograms[pName] = LoadProgramWithShaders(pShaders);
			return mShaderPrograms[pName];
		}
		return mShaderPrograms[pName];
	}

	Material::IMaterial* AssetsManager::LoadMaterial(const std::string& pId, bool pForceReleoad)
	{
		if (!mMaterials.contains(pId) || pForceReleoad) {
			mMaterials[pId] = LoadMaterialFromFile(pId);
			return mMaterials[pId];
		}
		return mMaterials[pId];
	}

	Material::IMaterial* AssetsManager::GetMaterial(const std::string& pId)
	{
		if (!mMaterials.contains(pId)) {
			std::ostringstream loadError;
			loadError << "Material " << pId << " does not exists in assets manager\n";
			ZP_CORE_ERROR(loadError.str());
		}
		return mMaterials[pId];
	}

	void AssetsManager::CleanUp()
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
		for (auto& iter : mCubemaps)
		{
			if (iter.second)
			{
				delete iter.second;
				iter.second = nullptr;
			}
		}
		mCubemaps.clear();
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

	ITexture2D* AssetsManager::LoadTextureFromFile(const std::string& pId)
	{
		return mContext->GetRenderer()->LoadTexture(mFileDataBase.GetPathFromID(pId), pId);
	}

	MeshData AssetsManager::LoadMeshData(const std::string& pId)
	{
		std::string path =mFileDataBase.GetPathFromID(pId);
		MeshData data;
		data.sourceFileId = pId;

		std::string extension = std::filesystem::path(path).extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".obj")
		{
			tinyobj::attrib_t attributes;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warning, errors;

			bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &errors, path.c_str());
			if (!success)
			{
				ZP_CORE_ERROR("Mesh " + path + " does not exist or is not .obj");
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
			ZP_LOAD("Mesh " + path + " successfully loaded");
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

	IFont* AssetsManager::LoadFontFromFile(const std::string& pId)
	{
		return mContext->GetRenderer()->LoadFont(mFileDataBase.GetPathFromID(pId), pId);
	}

	Render::IShader* AssetsManager::LoadShaderFromFile(const std::string& pId, ShaderType pType)
	{
		return mContext->GetRenderer()->LoadShader(mFileDataBase.GetPathFromID(pId), pId, pType);
	}

	Render::IShaderProgram* AssetsManager::LoadProgramWithShaders(const std::vector<Render::IShader*>& pShaders) const
	{
		return mContext->GetRenderer()->LoadShaderProgram(pShaders);
	}

	ICubeMapTexture* AssetsManager::LoadCubemapFromFile(const std::vector<std::string>& pCubeIds) const
	{		
		return mContext->GetRenderer()->LoadCubemap(pCubeIds);
	}

	Material::IMaterial* AssetsManager::LoadMaterialFromFile(const std::string& pMaterialFileId)
	{
		auto mat = mContext->GetRenderer()->CreateMaterial();
		mat->SetMaterialFileId(pMaterialFileId);
		std::string filePath = mFileDataBase.GetPathFromID(pMaterialFileId);
		auto reader = mContext->GetSerializationFactory()->CreateDeserializer();
		if (reader->LoadDocument(filePath))
		{
			mat->Deserialize(*reader);
		}
		else
		{
			return nullptr;
		}
		return mat;
	}

	// std::string AssetsManager::GetFullPath(const std::string& pPath, AssetType pType)
	// {
	// 	// TODO : Refactor this
	// 	std::string newPath;
	// 	switch (pType)
	// 	{
	// 	case AssetType::Texture:
	// 	{
	// 		if (pPath.find(IMPORT_PATH) == std::string::npos)
	// 		{
	// 			newPath = IMPORT_PATH + pPath;
	// 			if (newPath.find(".png") == std::string::npos && newPath.find(".jpg") == std::string::npos)
	// 			{
	// 				newPath = pPath;
	// 			}
	// 			break;
	// 		}
	// 		newPath = pPath;
	// 		break;
	// 	}
	// 	case AssetType::Mesh:
	// 	{
	// 		if (pPath.find(MESH_PATH) == std::string::npos)
	// 		{
	// 			newPath = MESH_PATH + pPath;
	// 			if (newPath.find(".obj") == std::string::npos)
	// 			{
	// 				newPath = pPath;
	// 			}
	// 			break;
	// 		}
	// 		newPath = pPath;
	// 		break;
	// 	}
	// 	case AssetType::Font:
	// 	{
	// 		if (pPath.find(FONT_PATH) == std::string::npos)
	// 		{
	// 			newPath = FONT_PATH + pPath;
	// 			break;
	// 		}
	// 		newPath = pPath;
	// 		break;
	// 	}
	// 	case AssetType::Shader:
	// 	{
	// 		if (pPath.find(SHADER_PATH) == std::string::npos)
	// 		{
	// 			newPath = SHADER_PATH + pPath;
	// 			if (newPath.find(".obj") == std::string::npos)
	// 			{
	// 				newPath = pPath;
	// 			}
	// 			break;
	// 		}
	// 		newPath = pPath;
	// 		break;
	// 	}
	// 	default:
	// 	{
	// 		newPath = pPath;
	// 		break;
	// 	}
	// 	}
	// 	return newPath;
	// 	//return pPath;
	// }
}
