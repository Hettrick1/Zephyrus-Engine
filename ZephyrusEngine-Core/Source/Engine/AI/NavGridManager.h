#pragma once

#include "Component/AIComponent/NavGridVolumeComponent.h"
#include "Vector3D.h"
#include "Debug/DebugLine.h"
#include <vector>
#include <map>

namespace Zephyrus::AI
{
	class Zephyrus::ActorComponent::NavGridVolumeComponent;
	
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
		
		void UpdateDebug();
	};
}


