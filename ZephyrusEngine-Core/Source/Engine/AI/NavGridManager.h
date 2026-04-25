#pragma once

#include "Component/AIComponent/NavGridVolumeComponent.h"
#include "Vector3D.h"
#include <vector>

namespace Zephyrus::AI
{
	class Zephyrus::ActorComponent::NavGridVolumeComponent;

	class NavGridManager
	{
		struct GridNode
		{
			Vector3D nodePosition = Vector3D::zero;
			bool isWalkable = true;
		};

	private:
		std::vector<ActorComponent::NavGridVolumeComponent*> mVolumeComponents;
		std::vector<std::vector<GridNode>> mGrid;
	public:
		NavGridManager() = default;
		~NavGridManager() = default;

		void AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);
		void RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);

		// it will compute only one grid, the first one in the component list. 
		// For now it uses a list of component to handle the case where I added a component before deleting the old one
		void ComputeGrid();
		
	};
}


