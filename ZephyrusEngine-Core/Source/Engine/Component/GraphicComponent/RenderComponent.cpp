#include "RenderComponent.h"
#include "Material/MaterialInstance.h"

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
