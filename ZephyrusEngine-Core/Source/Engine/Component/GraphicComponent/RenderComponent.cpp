#include "RenderComponent.h"

#include "ISerializationFactory.h"
#include "Material/MaterialInstance.h"
#include "Assets/Assets.h"

namespace Zephyrus::ActorComponent
{
	RenderComponent::RenderComponent(Actor* pOwner, const std::string& pName, const std::string& pDefaultMat)
		: Component(pOwner, pName)
	{
		mMaterial = Material::MaterialInstance();
		auto mat = Assets::AssetsManager::LoadMaterial(pDefaultMat, pDefaultMat);
		SetMaterial(mat);
		auto reader = mOwner->GetSceneContext()->GetSerializationFactory()->CreateDeserializer();
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
				mMaterial.Deserialize(pReader);
			}
			pReader.EndObject();
		}
	}

	void RenderComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		if (mMaterial.GetBaseMaterial())
		{
			mMaterial.Serialize(pWriter);
		}
	}

	void RenderComponent::SetMaterial(Material::IMaterial* newMaterial)
	{
		if (mMaterial.GetBaseMaterial() == newMaterial)
		{
			return;
		}
		mMaterial.SetMaterial(newMaterial);
	}
}
