#include "SkySphereComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "Interface/IMesh.h"

#include "ComponentFactory.h"
#include "JSONUtils.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	int SkySphereComponent::index = 0;

	SkySphereComponent::SkySphereComponent(Actor* pOwner)
		: RenderComponent(pOwner, "SkySphereComponent"), mMesh(nullptr), mIsSphere(false)
	{
		
	}

	void SkySphereComponent::OnStart()
	{
		Component::OnStart();
		mOwner->GetScene().GetRenderer()->AddSkySphere(this);
	}

	void SkySphereComponent::OnEnd()
	{
		mOwner->GetScene().GetRenderer()->RemoveSkySphere(this);
		Component::OnEnd();
	}

	void SkySphereComponent::Draw(const Zephyrus::Render::IRenderer& pRenderer)
	{
		if (mIsSphere)
		{
			mOwner->GetSceneContext()->GetRenderer()->DrawSkySphere(mMaterial, mMesh, GetWorldTransform());
		}
		else
		{
			mOwner->GetSceneContext()->GetRenderer()->DrawSkyBox(mMaterial, mMesh, GetWorldTransform());
		}
	}

	std::vector<PropertyDescriptor> SkySphereComponent::GetProperties()
	{
		return 
		{ 
			{ "Material Instance : ", &mMaterial, PropertyType::MaterialInstance }
		};
	}

	void SkySphereComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		RenderComponent::Deserialize(pReader);
		if (auto isSphere = pReader.ReadBool("isSphere"))
		{
			if (*isSphere) 
			{
				mIsSphere = true;
				mMesh = AssetsManager::LoadMesh("sphere.obj", "sphere");
			}
			else
			{
				mIsSphere = false;
				mMesh = AssetsManager::LoadMesh("cube.obj", "cube");
			}
		}
	}

	void SkySphereComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);

		pWriter.WriteBool("isSphere", mIsSphere);

		RenderComponent::Serialize(pWriter);
		Component::EndSerialize(pWriter);
	}

	void SkySphereComponent::SetTexturePaths(std::vector<std::string>& pTexturesPaths)
	{
		mTexturesPaths = pTexturesPaths;
	}
}