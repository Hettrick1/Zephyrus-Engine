#include "Assets.h"
#include <sstream>
#include "Log.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "MeshLoader/tiny_obj_loader.h"
#include "SceneManager.h"
#include "Scene.h"

std::map<std::string, Texture> Assets::mTextures = {};
std::map<std::string, Font> Assets::mFonts = {};
std::map<std::string, Mesh*> Assets::mMeshes = {};
std::map<std::string, Shader> Assets::mShaders = {};

const std::string Assets::IMPORT_PATH = "../Imports/";
const std::string Assets::MESH_PATH = "../Imports/Meshes/";
const std::string Assets::FONT_PATH = "../Imports/Fonts/";
const std::string Assets::SHADER_PATH = "../Imports/Shaders/";

Texture* Assets::LoadTexture(const std::string& pFilePath, const std::string& pName)
{
	if (mTextures.find(pName) == mTextures.end()) {
		mTextures[pName] = LoadTextureFromFile(*SceneManager::ActiveScene->GetRenderer(), IMPORT_PATH + pFilePath);
		return &mTextures[pName];
	}
	return &mTextures[pName];
}

Texture& Assets::GetTexture(const std::string& pName)
{
	if(mTextures.find(pName) == mTextures.end()) {
		std::ostringstream loadError;
		loadError << "Texture " << pName << " does not exists in assets manager\n";
		Log::Error(LogType::Application, loadError.str());
	}
	return mTextures[pName];
}

Mesh* Assets::LoadMesh(const std::string& pFilePath, const std::string& pName)
{
	if (mMeshes.find(pName) == mMeshes.end()) {
		mMeshes[pName] = LoadMeshFromFile(MESH_PATH + pFilePath);
		return mMeshes[pName];
	}
	return mMeshes[pName];
}

Mesh* Assets::GetMesh(const std::string& pName)
{
	if (mMeshes.find(pName) == mMeshes.end()) {
		std::ostringstream loadError;
		loadError << "Mesh " << pName << " does not exists in assets manager\n";
		Log::Error(LogType::Application, loadError.str());
	}
	return mMeshes[pName];
}

Font* Assets::LoadFont(const std::string& pFilePath, const std::string& pName)
{
	if (mFonts.find(pName) == mFonts.end()) {
		mFonts[pName] = LoadFontFromFile(FONT_PATH + pFilePath);
		return &mFonts[pName];
	}
	return &mFonts[pName];
}

Font& Assets::GetFont(const std::string& pName)
{
	if (mFonts.find(pName) == mFonts.end()) {
		std::ostringstream loadError;
		loadError << "Font " << pName << " does not exists in assets manager\n";
		Log::Error(LogType::Application, loadError.str());
	}
	return mFonts[pName];
}

Shader* Assets::LoadShader(const std::string& pFilePath, ShaderType pType, const std::string& pName)
{
	if (mShaders.find(pName) == mShaders.end()) {
		mShaders[pName] = LoadShaderFromFile(SHADER_PATH + pFilePath, pType);
		return &mShaders[pName];
	}
	return &mShaders[pName];
}

Shader& Assets::GetShader(const std::string& pName)
{
	if (mShaders.find(pName) == mShaders.end()) {
		std::ostringstream loadError;
		loadError << "Shader " << pName << " does not exists in assets manager\n";
		Log::Error(LogType::Application, loadError.str());
	}
	return mShaders[pName];
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
}

Texture Assets::LoadTextureFromFile(IRenderer& pRenderer, const std::string& pFilePath)
{
	Texture texture;
	texture.Load(pRenderer, pFilePath);
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
		Log::Error(LogType::Application, "Mesh " + pFilePath + " does not exist or is not .obj");
		return new Mesh();
	}
	else
	{
		Log::Info("Mesh " + pFilePath + " successfully loaded");
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
	return new Mesh(vertices);
}

Font Assets::LoadFontFromFile(const std::string& pFilePath)
{
	Font font;
	font.Load(pFilePath);
	return font;
}

Shader Assets::LoadShaderFromFile(const std::string& pFilePath, ShaderType pType)
{
	Shader shader;
	shader.Load(pFilePath, pType);
	std::cout << "Shader Load From assets : " + pFilePath << std::endl;
	return shader;
}
