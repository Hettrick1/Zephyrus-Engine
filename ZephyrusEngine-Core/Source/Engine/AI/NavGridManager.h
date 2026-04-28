#pragma once

#include "Component/AIComponent/NavGridVolumeComponent.h"
#include "Vector3D.h"
#include <vector>
#include <map>

namespace Zephyrus::AI
{
	class Zephyrus::ActorComponent::NavGridVolumeComponent;
	
	class NavGridManager
	{
		struct GridNode
		{
			GridNode() = default;
			~GridNode() = default;
			Vector3D nodePosition = Vector3D::zero;
			bool isWalkable = true;
			
			std::vector<GridNode*> neighbors; 
			
			float gCost = 0.0f;
			float hCost= 0.0f;
			GridNode* parent = nullptr;
		};
		
		struct GridCoord
		{
			GridCoord(int x, int y) : x(x), y(y) {}
			~GridCoord() = default;
			int x;
			int y;
			
			friend auto operator<=>(const GridCoord& lhs, const GridCoord& rhs) = default;
		};
	
		
	private:
		std::vector<ActorComponent::NavGridVolumeComponent*> mVolumeComponents;
		std::map<GridCoord, std::vector<GridNode>> mGrid;
		Vector3D StoredNodeSize;
		
		std::vector<Debug::DebugLine> mDebugLines;
		
		ISceneContext* mContext;
	public:
		explicit NavGridManager(ISceneContext* context);
		~NavGridManager();

		void AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);
		void RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);

		// it will compute only one grid, the first one in the component list. 
		// For now it uses a list of component to handle the case where I added a component before deleting the old one
		void ComputeGrid();
		
		void UpdateDebug();
	};
}


