#pragma once

#include "Component/AIComponent/NavGridVolumeComponent.h"
#include "Vector3D.h"
#include "Debug/DebugLine.h"
#include <vector>
#include <map>

namespace Zephyrus::AI
{
	class Zephyrus::ActorComponent::NavGridVolumeComponent;
	
	struct GridNode
	{
		GridNode() = default;
		~GridNode() = default;
		Vector3D nodePosition = Vector3D::zero;
		bool isWalkable = true;

		unsigned gridX = 0u;
		unsigned gridY = 0u;

		std::vector<GridNode*> neighbors;

		float g = 0.0f;
		float h = 0.0f;
		float f = 0.0f;
		float weight = 1.0f;

		bool isClosed = false;

		GridNode* parent = nullptr;

		float GetDistance(GridNode* from)
		{
			return nodePosition.Distance(from->nodePosition);
		}
	};

	class NavGridManager
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;

	public:
		explicit NavGridManager(ISceneContext* context);
		~NavGridManager();

		void AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);
		void RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent);

		// it will compute only one grid, the first one in the component list. 
		// For now it uses a list of component to handle the case where I added a component before deleting the old one
		void ComputeGrid();
		
		GridNode* GetNearestNodeFromWorldPosition(const Vector3D& pWorldLocation);

		std::vector<GridNode*> GetShortestPath(GridNode* startNode, GridNode* targetNode);
		void SmoothPath(std::vector<GridNode*>& inOutPath);

		void UpdateDebug();
	};
}


