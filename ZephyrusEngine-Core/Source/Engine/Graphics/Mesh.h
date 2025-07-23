#pragma once

#include <vector>
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"
#include "Vertex.h"

class Texture;
class VertexArray;

// Represents a 3D mesh with vertices, textures, and a vertex array object for rendering.
class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> pVertices);
	~Mesh();

	// Releases resources used by the mesh.
	void Unload();

	// Adds a texture to the mesh.
	void AddTexture(Texture* pTexture);

	VertexArray* GetVao() const
	{
		return mVao;
	}

	// Returns the texture at the given index.
	Texture* GetTexture(unsigned int pTextureIndex);

	// Converts the mesh's vertices to a float array.
	float* ToVerticeArray();

	inline unsigned int GetTextureArraySize() const { return mTextures.size(); }

private:
	std::vector<Texture*> mTextures;
	std::vector<Vertex> mVertices;
	VertexArray* mVao = nullptr;
};
