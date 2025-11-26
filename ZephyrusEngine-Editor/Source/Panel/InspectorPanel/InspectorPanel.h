#pragma once
#include "../Panel.h"
#include "../SceneHierarchyPanel.h"
#include "CameraComponent.h"
#include "ComponentPropertyDrawer.h"
#include "ActorDrawer.h"

#include <unordered_map>
#include <functional>

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::ActorState;

class Component;
struct PropertyDescriptor;

class InspectorPanel : public Panel
{
private:
	SceneHierarchyPanel* mHierarchy{ nullptr };
	Component* mActiveComponent{ nullptr };
	ComponentPropertyDrawer* mComponentPropertyDrawer{ nullptr };
	ActorDrawer* mActorDrawer{ nullptr };
public:
	InspectorPanel(ISceneContext* pSceneContext, const std::string& pName);
	~InspectorPanel() override;
	void Draw() override;
	
	void DrawComponentInfos(Actor* pActor);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);

	inline CameraComponent* GetCurrentCameraComponent() const
	{
		if (!mActiveComponent)
		{
			return nullptr;
		}
		CameraComponent* cameraComp = dynamic_cast<CameraComponent*>(mActiveComponent);
		if (cameraComp)
		{
			return cameraComp;
		}
		return nullptr;
	}

	inline void ResetActiveComponent() 
	{
		mActiveComponent = nullptr;
	}
};
