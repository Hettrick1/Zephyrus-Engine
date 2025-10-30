#include "MeshComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "DebugRenderer.h"
#include "JSONUtils.h"
#include "Interface/ITexture.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	MeshComponent::MeshComponent(Actor* pOwner)
		: Component(pOwner, "MeshComponent"), mMesh(nullptr), mTiling(Vector2D(pOwner->GetTransformComponent().GetSize().x, pOwner->GetTransformComponent().GetSize().y))
	{
		mOwner->GetScene().GetRenderer()->AddMesh(this);

		mVertexShader = AssetsManager::LoadShader("BasicMesh.vert", ShaderType::VERTEX, "basicMeshVert");
		mFragmentShader = AssetsManager::LoadShader("BasicMesh.frag", ShaderType::FRAGMENT, "basicMeshFrag");
		mShaderProgram = AssetsManager::LoadShaderProgram({ mVertexShader, mFragmentShader }, "basicMeshSP");

		mOutlineVertexShader = AssetsManager::LoadShader("BasicOutline.vert", ShaderType::VERTEX, "OutlineVert");
		mOutlineFragmentShader = AssetsManager::LoadShader("BasicOutline.frag", ShaderType::FRAGMENT, "OutlineFrag");
		mOutlineShaderProgram = AssetsManager::LoadShaderProgram({ mOutlineVertexShader, mOutlineFragmentShader }, "OutlineSP");

		mMesh = AssetsManager::LoadMesh("cube.obj", "cube.obj");
		auto texture = AssetsManager::LoadTexture("../Content/Sprites/planks.png", "../Content/Sprites/planks.png");
		AddTexture(texture);
		SetTextureIndex(0);
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (auto mesh = pReader.ReadString("mesh"))
		{
			mMesh = AssetsManager::LoadMesh(*mesh, *mesh);
			if (!mMesh)
			{
				ZP_CORE_ERROR("Mesh creation failed !");
			}
			if (auto textures = pReader.ReadArrayString("textures"))
			{
				mTextures.clear();
				const auto& arr = *textures;

				if (!arr.empty())
				{
					for (auto& element : arr)
					{
						Assets::ITexture* texture = AssetsManager::LoadTexture(element, element);
						AddTexture(texture);
					}
				}
			}
			if (auto index = pReader.ReadInt("textureIndex"))
			{
				SetTextureIndex(*index);
				ZP_INFO(mOwner->GetName() + std::to_string(*index));
			}
			else
			{
				SetTextureIndex(0);
			}
			if (auto tiling = pReader.ReadVector2D("textureTiling"))
			{
				SetTiling(*tiling);
			}
		}
		else
		{
			mMesh = AssetsManager::LoadMesh("cube.obj", "cube.obj");
			SetTextureIndex(0);
			ZP_CORE_WARN("No mesh referenced in the prefab actor !");
		}
	}

	void MeshComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("mesh", mMesh->GetFilePath());
		pWriter.BeginArray("textures");
		for (auto& texture : GetAllTextures())
		{
			pWriter.PushString(texture->GetFilePath());
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
		Component::OnEnd();
	}

	std::vector<PropertyDescriptor> MeshComponent::GetProperties()
	{
		return
		{
			{ "Mesh : ", &mMesh, PropertyType::Mesh },
			{ "Textures : ", &mTextures, PropertyType::VectorTexture },
			{ "Texture Index : ", &mTextureIndex, PropertyType::Int },
			{ "Texture Tiling : ", &mTiling, PropertyType::Vec2 },
		};
	}

	void MeshComponent::Draw(const Matrix4DRow& pViewProj)
	{
		if (!mMesh)
		{
			return;
		}

		Matrix4DRow wt = GetWorldTransform();
		mShaderProgram->Use();
		mShaderProgram->setMatrix4Row("uViewProj", pViewProj);
		mShaderProgram->setMatrix4Row("uWorldTransform", wt);
		mShaderProgram->setVector2f("uTiling", mTiling);
		if (GetTextureArraySize() < mTextureIndex)
		{
			mTextureIndex = 0;
		}
		Assets::ITexture* tex = GetTexture(mTextureIndex);
		if (tex)
		{
			tex->Bind();
		}
		mMesh->Bind();
		if ((mShaderProgram->GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
		{
			glDrawArrays(GL_PATCHES, 0, mMesh->GetVertexCount());
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, mMesh->GetVertexCount());
		}
	}

	void MeshComponent::DrawSelected(const Matrix4DRow& pViewProj)
	{

	}

	void MeshComponent::SetMesh(IMesh* pMesh)
	{
		mMesh = pMesh;
	}

	void MeshComponent::SetTextureIndex(unsigned int pTextureIndex)
	{
		if (pTextureIndex < GetTextureArraySize())
		{
			mTextureIndex = pTextureIndex;
		}
		else {
			std::cout << "The texture index is out of bounds !" << std::endl;
			mTextureIndex = 0;
		}
	}

	void MeshComponent::AddTexture(Assets::ITexture* pTexture)
	{
		if (std::find(mTextures.begin(), mTextures.end(), pTexture) == mTextures.end())
		{
			mTextures.push_back(pTexture);
		}
	}

	Assets::ITexture* MeshComponent::GetTexture(unsigned int pTextureIndex)
	{
		if (pTextureIndex < GetTextureArraySize())
		{
			return mTextures[pTextureIndex];
		}
		else
		{
			mTextureIndex = 0;
			return mTextures[mTextureIndex];
		}
	}


	void MeshComponent::SetShaderProgram(Render::IShaderProgram* pShaderProgram)
	{
		mShaderProgram = pShaderProgram;
	}

	void MeshComponent::SetTiling(const Vector2D& pTiling)
	{
		mTiling = pTiling;
	}
}
