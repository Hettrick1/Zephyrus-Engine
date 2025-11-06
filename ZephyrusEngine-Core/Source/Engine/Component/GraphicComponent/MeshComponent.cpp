#include "MeshComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "DebugRenderer.h"
#include "JSONUtils.h"
#include "Interface/ITexture2D.h"


using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	MeshComponent::MeshComponent(Actor* pOwner)
		: RenderComponent(pOwner, "MeshComponent"), mMesh(nullptr), mTiling(Vector2D(pOwner->GetTransformComponent().GetSize().x, pOwner->GetTransformComponent().GetSize().y))
	{
		mOwner->GetScene().GetRenderer()->AddMesh(this);

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
		RenderComponent::Deserialize(pReader);
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
						Assets::ITexture2D* texture = AssetsManager::LoadTexture(element, element);
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

		if (!mMaterial) return;
		// TODO serialize materials
		mMaterial->SetTexture("albedo", mTextures[mTextureIndex]);
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
		RenderComponent::Serialize(pWriter);
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

	void MeshComponent::Draw(const Zephyrus::Render::IRenderer& pRenderer)
	{
		if (!mMesh)
		{
			return;
		}
		pRenderer.DrawMesh(mMaterial, mMesh, GetWorldTransform());
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

	void MeshComponent::AddTexture(Assets::ITexture2D* pTexture)
	{
		if (std::find(mTextures.begin(), mTextures.end(), pTexture) == mTextures.end())
		{
			mTextures.push_back(pTexture);
		}
	}

	Assets::ITexture2D* MeshComponent::GetTexture(unsigned int pTextureIndex)
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

	void MeshComponent::SetTiling(const Vector2D& pTiling)
	{
		mTiling = pTiling;

		if (!mMaterial) return;
		// TODO serialize materials (need tiling to be in the material instance)
		mMaterial->SetVector2D("uTiling", pTiling);
	}
}
