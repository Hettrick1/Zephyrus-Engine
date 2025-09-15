#include "Assets.h"
#include <sstream>
#include "Log.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "SceneManager.h"
#include "Scene.h"

std::map<std::string, Texture> Assets::mTextures = {};
std::map<std::string, Font> Assets::mFonts = {};
std::map<std::string, Mesh*> Assets::mMeshes = {};
std::map<std::string, Shader> Assets::mShaders = {};
std::map<std::string, ShaderProgram> Assets::mShaderPrograms = {};

const std::string Assets::IMPORT_PATH = "../Content/";
const std::string Assets::MESH_PATH = "../Content/Meshes/";
const std::string Assets::FONT_PATH = "../Content/Fonts/";
const std::string Assets::SHADER_PATH = "../Content/Shaders/";

Texture* Assets::LoadTexture(const std::string& pFilePath, const std::string& pName)
{
	if (mTextures.find(pName) == mTextures.end()) {
		mTextures[pName] = LoadTextureFromFile(*SceneManager::ActiveScene->GetRenderer(), GetFullPath(pFilePath, AssetType::Texture));
		return &mTextures[pName];
	}
	return &mTextures[pName];
}

Texture& Assets::GetTexture(const std::string& pName)
{
	if(mTextures.find(pName) == mTextures.end()) {
		std::ostringstream loadError;
		loadError << "Texture " << pName << " does not exists in assets manager\n";
		ZP_CORE_ERROR(loadError.str());
	}
	return mTextures[pName];
}

Mesh* Assets::LoadMesh(const std::string& pFilePath, const std::string& pName)
{
	if (mMeshes.find(pName) == mMeshes.end()) {
		mMeshes[pName] = LoadMeshFromFile(GetFullPath(pFilePath, AssetType::Mesh));
		return mMeshes[pName];
	}
	return mMeshes[pName];
}

Mesh* Assets::GetMesh(const std::string& pName)
{
	if (mMeshes.find(pName) == mMeshes.end()) {
		std::ostringstream loadError;
		loadError << "Mesh " << pName << " does not exists in assets manager\n";
		ZP_CORE_ERROR(loadError.str());
	}
	return mMeshes[pName];
}

Font* Assets::LoadFont(const std::string& pFilePath, const std::string& pName)
{
	if (mFonts.find(pName) == mFonts.end()) {
		mFonts[pName] = LoadFontFromFile(GetFullPath(pFilePath, AssetType::Font));
		return &mFonts[pName];
	}
	return &mFonts[pName];
}

Font& Assets::GetFont(const std::string& pName)
{
	if (mFonts.find(pName) == mFonts.end()) {
		std::ostringstream loadError;
		loadError << "Font " << pName << " does not exists in assets manager\n";
		ZP_CORE_ERROR(loadError.str());
	}
	return mFonts[pName];
}

Shader* Assets::LoadShader(const std::string& pFilePath, ShaderType pType, const std::string& pName)
{
	if (mShaders.find(pName) == mShaders.end()) {
		mShaders[pName] = LoadShaderFromFile(GetFullPath(pFilePath, AssetType::Shader), pType);
		return &mShaders[pName];
	}
	return &mShaders[pName];
}

Shader& Assets::GetShader(const std::string& pName)
{
	if (mShaders.find(pName) == mShaders.end()) {
		std::ostringstream loadError;
		loadError << "Shader " << pName << " does not exists in assets manager\n";
		ZP_CORE_ERROR(loadError.str());
	}
	return mShaders[pName];
}

ShaderProgram* Assets::LoadShaderProgram(std::vector<Shader*> pShaders, const std::string& pName)
{
	if (mShaderPrograms.find(pName) == mShaderPrograms.end()) {
		ShaderProgram program = ShaderProgram();
		program.Compose(pShaders);
		mShaderPrograms[pName] = program;
		return &mShaderPrograms[pName];
	}
	return &mShaderPrograms[pName];
}

void Assets::Clear()
{
	for (auto& iter : mTextures)
	{
		iter.second.Unload(); 
	}
	mTextures.clear();
	for (auto& iter : mMeshes)
	{
		iter.second->Unload();
		delete iter.second;
	}
	mMeshes.clear();
	for (auto& iter : mFonts)
	{
		iter.second.Unload();
	}
	mFonts.clear();
	for (auto& iter : mShaders)
	{
		iter.second.Unload();
	}
	mShaders.clear();
	for (auto& iter : mShaderPrograms)
	{
		iter.second.Unload();
	}
	mShaderPrograms.clear();
}

Texture Assets::LoadTextureFromFile(IRenderer& pRenderer, const std::string& pFilePath)
{
	Texture texture;
	texture.Load(pRenderer, pFilePath);
	ZP_LOAD("Texture " + pFilePath + " successfully loaded");
	return texture;
}

Mesh* Assets::LoadMeshFromFile(const std::string& pFilePath)
{
	Mesh loaded; 
	tinyobj::attrib_t attributes; 
	std::vector<tinyobj::shape_t> shapes; 
	std::vector<tinyobj::material_t> materials; 
	std::string warning, errors; 
	std::string dir = "Meshes/"; 
	bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &errors, + pFilePath.c_str());
	if (!success) 
	{
		ZP_CORE_ERROR("Mesh " + pFilePath + " does not exist or is not .obj");
		return new Mesh();
	}
	else
	{
		ZP_LOAD("Mesh " + pFilePath + " successfully loaded");
	}
	std::vector<Vertex> vertices;
	for (int i = 0; i < static_cast<int>(shapes.size()); i++)
	{
		tinyobj::shape_t& shape = shapes[i];
		tinyobj::mesh_t& mesh = shape.mesh;
		for (int j = 0; j < static_cast<int>(mesh.indices.size()); j++)
		{
			tinyobj::index_t i = mesh.indices[j];
			Vector3D position = Vector3D{
				attributes.vertices[i.vertex_index * 3],
				attributes.vertices[i.vertex_index * 3 + 1], 
				attributes.vertices[i.vertex_index * 3 + 2]
			};
			Vector3D normal = Vector3D{
				attributes.normals[i.normal_index * 3],
				attributes.normals[i.normal_index * 3 + 1],
				attributes.normals[i.normal_index * 3 + 2]
			};
			Vector2D texCoord = {
				attributes.texcoords[i.texcoord_index * 2],
				attributes.texcoords[i.texcoord_index * 2 + 1],
			};
			Vertex vert = { position, normal, texCoord };
			vertices.push_back(vert);
		}

	}
	return new Mesh(vertices, pFilePath);
}

Font Assets::LoadFontFromFile(const std::string& pFilePath)
{
	Font font;
	font.Load(pFilePath);
	ZP_LOAD("Font " + pFilePath + " successfully loaded");
	return font;
}

Shader Assets::LoadShaderFromFile(const std::string& pFilePath, ShaderType pType)
{
	Shader shader;
	shader.Load(pFilePath, pType);
	ZP_LOAD("Shader " + pFilePath + " successfully loaded");
	return shader;
}

std::string Assets::GetFullPath(const std::string& pPath, AssetType pType)
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
