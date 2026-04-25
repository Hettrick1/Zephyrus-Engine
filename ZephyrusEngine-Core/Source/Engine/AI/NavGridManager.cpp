#include "pch.h"
#include "NavGridManager.h"

namespace Zephyrus::AI
{
	void NavGridManager::AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		if (std::find(mVolumeComponents.begin(), mVolumeComponents.end(), pComponent) == mVolumeComponents.end())
		{
			mVolumeComponents.emplace_back(pComponent);
		}
	}
	void NavGridManager::RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		std::erase(mVolumeComponents, pComponent);
	}
	void NavGridManager::ComputeGrid()
	{
		ZP_EDITOR_INFO("Computing the grid");
	}
}