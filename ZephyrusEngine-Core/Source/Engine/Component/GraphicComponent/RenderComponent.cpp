#include "pch.h"
#include "RenderComponent.h"

#include "ISerializationFactory.h"
#include "Material/MaterialInstance.h"
#include "Assets/AssetsManager.h"

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
		ZP_EDITOR_WARN("begin render comp : " + std::to_string(pReader.GetStackCount()));
		if (pReader.BeginObject("materialInstance"))
		{
			ZP_EDITOR_WARN("begin mat : " + std::to_string(pReader.GetStackCount()));
			if (auto materialPath = pReader.ReadString("baseMaterial"))
			{
				auto mat = Assets::AssetsManager::LoadMaterial(*materialPath, *materialPath);
				SetMaterial(mat);
				mMaterial.Deserialize(pReader);
			}
			ZP_EDITOR_WARN("end mat : " + std::to_string(pReader.GetStackCount()));
			pReader.EndObject();
		}
		ZP_EDITOR_WARN("end render comp : " + std::to_string(pReader.GetStackCount()));
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
