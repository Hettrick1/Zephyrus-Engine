#pragma once

#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Vertex.h"

class Texture;
class VertexArray;

// Represents a 3D mesh with vertices, textures, and a vertex array object for rendering.
class Mesh
{
private:
	std::vector<Texture*> mTextures;
	std::vector<Vertex> mVertices;
	VertexArray* mVao = nullptr;
	std::string mFilePath;
public:
	Mesh();
	Mesh(std::vector<Vertex> pVertices, const std::string pFilePath);
	~Mesh();

	// Releases resources used by the mesh.
	void Unload();

	// Adds a texture to the mesh.
	void AddTexture(Texture* pTexture);

	// Returns the texture at the given index.
	Texture* GetTexture(unsigned int pTextureIndex);

	// Converts the mesh's vertices to a float array.
	float* ToVerticeArray();

	inline VertexArray* GetVao() const { return mVao; }
	inline unsigned int GetTextureArraySize() const { return mTextures.size(); }
	inline std::string GetMeshFilePath() const { return mFilePath; }
	inline std::vector<Texture*> GetAllTextures() const { return mTextures; }
};
