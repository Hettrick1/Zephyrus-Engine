#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Assets.h"
#include "Scene.h"
#include "SceneManager.h"
namespace Zephyrus::Assets {
	Mesh::Mesh()
		: mVao(nullptr)
	{
	}

	Mesh::Mesh(std::vector<Vertex> pVertices, const std::string pFilePath)
		: mVertices(std::move(pVertices)), mVao(nullptr), mFilePath(pFilePath)
	{

		mVao = new VertexArrayOpenGL(ToVerticeArray(), mVertices.size());
	}

	Mesh::~Mesh()
	{
		Unload();
	}

	void Mesh::Unload()
	{
		if (mVao != nullptr) {
			delete mVao;
		}
		mVao = nullptr;
	}

	float* Mesh::ToVerticeArray()
	{
		if (mVertices.empty()) return nullptr;

		float* array = new float[mVertices.size() * 8];
		for (size_t i = 0; i < mVertices.size(); i++)
		{
			size_t index = i * 8;
			array[index + 0] = mVertices[i].position.x;
			array[index + 1] = mVertices[i].position.z;// inverser y et z car le up est différent
			array[index + 2] = mVertices[i].position.y;
			array[index + 3] = mVertices[i].normal.x;
			array[index + 4] = mVertices[i].normal.z;
			array[index + 5] = mVertices[i].normal.y;
			array[index + 6] = mVertices[i].texCoord.x;
			array[index + 7] = -mVertices[i].texCoord.y;
		}
		return array;
	}
}