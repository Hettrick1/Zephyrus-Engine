#pragma once

#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Vertex.h"


using Zephyrus::Render::Vertex;

// Represents a 3D mesh with vertices, textures, and a vertex array object for rendering.
namespace Zephyrus::Assets {
	class VertexArray;
	class Texture;
	class Mesh
	{
	private:
		std::vector<Vertex> mVertices;
		VertexArray* mVao = nullptr;
		std::string mFilePath;
	public:
		Mesh();
		Mesh(std::vector<Vertex> pVertices, const std::string pFilePath);
		~Mesh();

		// Releases resources used by the mesh.
		void Unload();

		// Converts the mesh's vertices to a float array.
		float* ToVerticeArray();

		inline VertexArray* GetVao() const { return mVao; }
		inline std::string GetMeshFilePath() const { return mFilePath; }
	};
}