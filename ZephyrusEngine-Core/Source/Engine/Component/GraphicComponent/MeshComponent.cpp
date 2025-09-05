#include "MeshComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"

MeshComponent::MeshComponent(Actor* pOwner)
	: Component(pOwner, "MeshComponent"), mMesh(nullptr), mTiling(Vector2D(pOwner->GetTransformComponent().GetSize().x, pOwner->GetTransformComponent().GetSize().y))
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
	mOwner->GetScene().GetRenderer()->AddMesh(this);
	mVertexShader = *Assets::LoadShader("BasicMesh.vert", ShaderType::VERTEX, "basicMeshVert");
	mFragmentShader = *Assets::LoadShader("BasicMesh.frag", ShaderType::FRAGMENT, "basicMeshFrag");
	mShaderProgram = *Assets::LoadShaderProgram({ &mVertexShader, &mFragmentShader }, "basicMeshSP");

	mOutlineVertexShader = *Assets::LoadShader("BasicOutline.vert", ShaderType::VERTEX, "OutlineVert");
	mOutlineFragmentShader = *Assets::LoadShader("BasicOutline.frag", ShaderType::FRAGMENT, "OutlineFrag");
	mOutlineShaderProgram = *Assets::LoadShaderProgram({ &mOutlineVertexShader, &mOutlineFragmentShader }, "OutlineSP");

	if (pData.HasMember("mesh") && pData["mesh"].IsString())
	{
		mMesh = Assets::LoadMesh(pData["mesh"].GetString(), pData["mesh"].GetString());
		if (!mMesh)
		{
			ZP_CORE_ERROR("Mesh creation failed !");
		}
		if (pData.HasMember("textures") && pData["textures"].IsArray())
		{
			const auto& arr = pData["textures"].GetArray();

			if (!arr.Empty())
			{
				for (auto& element : arr)
				{
					if (element.IsString())
					{
						Texture* texture = Assets::LoadTexture(element.GetString(), element.GetString());
						mMesh->AddTexture(texture);
					}
				}
			}
		}
		if (pData.HasMember("textureIndex") && pData["textureIndex"].IsInt())
		{
			SetTextureIndex(pData["textureIndex"].GetInt());
		}
		else
		{
			SetTextureIndex(0);
		}
	}
	else 
	{
		mMesh = Assets::LoadMesh("cube.obj", "cube.obj");
		SetTextureIndex(0);
		ZP_CORE_WARN("No mesh referenced in the prefab actor !");
	}
}

void MeshComponent::Draw(const Matrix4DRow& pViewProj)
{
	if (!mMesh)
		return;

	Matrix4DRow wt = mOwner->GetTransformComponent().GetWorldTransform();
	mShaderProgram.Use();
	mShaderProgram.setMatrix4Row("uViewProj", pViewProj);
	mShaderProgram.setMatrix4Row("uWorldTransform", wt);
	mShaderProgram.setVector2f("uTiling", mTiling);
	Texture* tex = mMesh->GetTexture(mTextureIndex);
	if (tex)
	{
		tex->SetActive();
	}
	mMesh->GetVao()->SetActive();
	if ((mShaderProgram.GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
	{
		glDrawArrays(GL_PATCHES, 0, mMesh->GetVao()->GetVerticeCount());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mMesh->GetVao()->GetVerticeCount());
	}
}

void MeshComponent::SetMesh(Mesh& pMesh)
{
	mMesh = &pMesh;
}

void MeshComponent::SetTextureIndex(unsigned int pTextureIndex)
{
	if (mTextureIndex < mMesh->GetTextureArraySize())
	{
		mTextureIndex = pTextureIndex;
	}
	else {
		std::cout << "The texture index is out of bounds !" << std::endl;
		mTextureIndex = 0;
	}
}

void MeshComponent::SetShaderProgram(const ShaderProgram& pShaderProgram)
{
	mShaderProgram = pShaderProgram;
}

void MeshComponent::SetTiling(const Vector2D& pTiling)
{
	mTiling = pTiling;
}
