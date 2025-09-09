#include "MeshComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "DebugRenderer.h"
#include "JSONUtils.h"

MeshComponent::MeshComponent(Actor* pOwner)
	: Component(pOwner, "MeshComponent"), mMesh(nullptr), mTiling(Vector2D(pOwner->GetTransformComponent().GetSize().x, pOwner->GetTransformComponent().GetSize().y))
{
	mOwner->GetScene().GetRenderer()->AddMesh(this);
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
	mVertexShader = *Assets::LoadShader("BasicMesh.vert", ShaderType::VERTEX, "basicMeshVert");
	mFragmentShader = *Assets::LoadShader("BasicMesh.frag", ShaderType::FRAGMENT, "basicMeshFrag");
	mShaderProgram = *Assets::LoadShaderProgram({ &mVertexShader, &mFragmentShader }, "basicMeshSP");

	mOutlineVertexShader = *Assets::LoadShader("BasicOutline.vert", ShaderType::VERTEX, "OutlineVert");
	mOutlineFragmentShader = *Assets::LoadShader("BasicOutline.frag", ShaderType::FRAGMENT, "OutlineFrag");
	mOutlineShaderProgram = *Assets::LoadShaderProgram({ &mOutlineVertexShader, &mOutlineFragmentShader }, "OutlineSP");

	if (auto mesh = Serialization::Json::ReadString(pData, "mesh"))
	{
		mMesh = Assets::LoadMesh(*mesh, *mesh);
		if (!mMesh)
		{
			ZP_CORE_ERROR("Mesh creation failed !");
		}
		if (auto textures = Serialization::Json::ReadArrayString(pData, "textures"))
		{
			const auto& arr = *textures;

			if (!arr.empty())
			{
				for (auto& element : arr)
				{
					Texture* texture = Assets::LoadTexture(element, element);
					mMesh->AddTexture(texture);
				}
			}
		}
		if (auto index = Serialization::Json::ReadInt(pData, "textureIndex"))
		{
			SetTextureIndex(*index);
			ZP_INFO(mOwner->GetName() + std::to_string(*index));
		}
		else
		{
			SetTextureIndex(0);
		}
		if (auto tiling = Serialization::Json::ReadVector2D(pData, "textureTiling"))
		{
			SetTiling(*tiling);
		}
	}
	else 
	{
		mMesh = Assets::LoadMesh("cube.obj", "cube.obj");
		SetTextureIndex(0);
		ZP_CORE_WARN("No mesh referenced in the prefab actor !");
	}
}

void MeshComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
	Component::BeginSerialize(pWriter);
	pWriter.WriteString("mesh", mMesh->GetMeshFilePath());
	pWriter.BeginArray("textures");
	for (auto& texture : mMesh->GetAllTextures())
	{
		pWriter.PushString(texture->GetTextureFilePath());
	}
	pWriter.EndArray();
	pWriter.WriteInt("textureIndex", mTextureIndex);
	pWriter.WriteVector2D("textureTiling", mTiling);
	Component::EndSerialize(pWriter);
}

void MeshComponent::OnStart()
{
}

void MeshComponent::OnEnd()
{
	mOwner->GetScene().GetRenderer()->RemoveMesh(this);
}

void MeshComponent::Draw(const Matrix4DRow& pViewProj)
{
	if (!mMesh)
	{
		return;
	}

	Matrix4DRow wt = GetWorldTransform();
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

void MeshComponent::DrawSelected(const Matrix4DRow& pViewProj)
{
	
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
