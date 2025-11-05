#include "RenderComponent.h"
#include "Material/MaterialInstance.h"
#include "Assets/Assets.h"

namespace Zephyrus::ActorComponent
{
	RenderComponent::RenderComponent(Actor* pOwner, const std::string& pName)
		: Component(pOwner, pName)
	{
	}

	RenderComponent::~RenderComponent()
	{
		if(mMaterial)
		{
			delete mMaterial;
			mMaterial = nullptr;
		}
	}

	void RenderComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (pReader.BeginObject("materialInstance"))
		{
			if (auto materialPath = pReader.ReadString("baseMaterial"))
			{
				auto mat = Assets::AssetsManager::LoadMaterial(*materialPath, *materialPath);
				SetMaterial(mat);
				mMaterial->Deserialize(pReader);
			}
			pReader.EndObject();
		}
	}

	void RenderComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		if (mMaterial)
		{
			mMaterial->Serialize(pWriter);
		}
	}

	void RenderComponent::SetMaterial(Material::IMaterial* newMaterial)
	{
		if (mMaterial)
		{
			if (mMaterial->GetBaseMaterial() == newMaterial)
			{
				return;
			}
			mMaterial->SetMaterial(newMaterial);
		}
		else
		{
			mMaterial = new Material::MaterialInstance(newMaterial);
		}
	}
}
