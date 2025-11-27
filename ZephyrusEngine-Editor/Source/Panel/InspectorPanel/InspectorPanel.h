#pragma once
#include "../Panel.h"
#include "../SceneHierarchyPanel.h"
#include "CameraComponent.h"
#include "ComponentPropertyDrawer.h"
#include "ActorDrawer.h"

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
	bool mKeepRatio{ true };
	Vector3D mOriginalSize{ 0.0f, 0.0f, 0.0f };
public:
	InspectorPanel(ISceneContext* pSceneContext, const std::string& pName);
	~InspectorPanel() override;
	void Draw() override;
	
	void DrawComponentInfos(Actor* pActor);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);

	void CreateSetComponentLocation(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetComponentRotation(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetComponentSize(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	
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
