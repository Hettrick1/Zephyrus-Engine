#include "pch.h"
#include "NavGridVolumeComponent.h"
#include "AI/NavGridManager.h"

namespace Zephyrus::ActorComponent
{
	NavGridVolumeComponent::NavGridVolumeComponent(Actor* pOwner)
		: Component(pOwner, "NavGridVolumeComponent")
	{
		mOwner->GetSceneContext()->GetNavGridManager()->AddVolumeComponent(this);
	}

	NavGridVolumeComponent::~NavGridVolumeComponent()
	{
		mOwner->GetSceneContext()->GetNavGridManager()->RemoveVolumeComponent(this);
	}

	void NavGridVolumeComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void NavGridVolumeComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		Component::EndSerialize(pWriter);
	}

	std::vector<PropertyDescriptor> NavGridVolumeComponent::GetProperties()
	{
		std::function<void()> computeCallback = [&]()
			{
				mOwner->GetSceneContext()->GetNavGridManager()->ComputeGrid();
			};

		const bool condition = mGridSize.x > 0 && mGridSize.y > 0 && mNodeSize > 0;

		return
		{
			{ "Grid Size : ", &mGridSize, PropertyType::Vec2},
			{ "Node Size : ", &mNodeSize, PropertyType::Float},
			{ "Compute Grid : ", nullptr, PropertyType::Button, Callback(computeCallback, "Compute") | Condition(condition)}
		};
	}

	void NavGridVolumeComponent::OnStart()
	{
	}

	void NavGridVolumeComponent::Update()
	{
	}

	void NavGridVolumeComponent::OnEnd()
	{
	}
}
