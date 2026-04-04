#include "pch.h"
#include "MeshComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "DebugRenderer.h"
#include "JSONUtils.h"
#include "Interface/ITexture2D.h"
#include "EngineContentIds.h"


using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	MeshComponent::MeshComponent(Actor* pOwner, const std::string& pName)
		: RenderComponent(pOwner, pName, MAT_DEFAULT_MESH), mMesh(nullptr)
	{
		mOwner->GetScene().GetRenderer()->AddMesh(this);

		mMesh = AssetsManager::GetInstance().LoadMesh(MESH_CUBE_OBJ);
	}

	void MeshComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		RenderComponent::Deserialize(pReader);
		if (auto mesh = pReader.ReadString("mesh"))
		{
			mMesh = AssetsManager::GetInstance().LoadMesh(*mesh);
			if (!mMesh)
			{
				ZP_CORE_ERROR("Mesh creation failed !");
			}
		}
		else
		{
			mMesh = AssetsManager::GetInstance().LoadMesh(MESH_CUBE_OBJ);
			ZP_CORE_WARN("No mesh referenced in " + mOwner->GetName());
		}
	}

	void MeshComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("mesh", mMesh->GetFileId());
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
			{ "Material Instance : ", &mMaterial, PropertyType::MaterialInstance},
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
}