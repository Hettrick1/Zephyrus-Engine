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

		const bool condition = mGridSize.x > 0 && mGridSize.y > 0 && mAgentWidth > 0 && mAgentHeight > 0;

		const Vector3D newSize = Vector3D((mAgentWidth) * mGridSize.x, (mAgentWidth) * mGridSize.y, mAgentHeight);
		mOwner->SetSize(newSize);
		
		return
		{
			{ "Grid Size : ", &mGridSize, PropertyType::Vec2},
			{ "Agent Width : ", &mAgentWidth, PropertyType::Float},
			{ "Agent Height : ", &mAgentHeight, PropertyType::Float},
			{ "Compute Grid : ", nullptr, PropertyType::Button, Callback(computeCallback, "Compute") | Condition(condition)},
			{ "Show line trace : ", &mShowLines, PropertyType::Bool},
			{ "Show position : ", &mShowNodePos, PropertyType::Bool},
		};
	}

	void NavGridVolumeComponent::OnStart()
	{
		const Vector3D newSize = Vector3D((mAgentWidth + 0.2f) * mGridSize.x, (mAgentWidth + 0.2f) * mGridSize.y, mAgentHeight);
		mOwner->SetSize(newSize);
	}

	void NavGridVolumeComponent::Update()
	{
	}

	void NavGridVolumeComponent::OnEnd()
	{
	}
}
