#pragma once

#include "Vector3D.h"
#include <functional>

namespace Zephyrus::Factory
{
	class ComponentFactory;
}

namespace Zephyrus::ActorComponent
{
	class Component;
}

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class ActorDrawer
{
private:
	Vector3D mOldActorTransform{ 0 };
	bool mKeepRatio{ true };
	Vector3D mOriginalSize{ 0 };

	float mLabelWidth{ 80.0f };

	unsigned int mSelected{ 0 };
	bool mSelfSelected{ true };
	Zephyrus::ActorComponent::Component* mActiveComponent{nullptr};
public:
	ActorDrawer() = default;
	~ActorDrawer() = default;

	void DrawActorInfos(Zephyrus::ActorComponent::Actor* pActor);
	void SetTransform(Zephyrus::ActorComponent::Actor* pActor, const std::string& label, float step, const Vector3D& initialValue, const std::function<void(const Vector3D&)>& realTimeSetter, const std::function<void(Zephyrus::ActorComponent::Actor*, const Vector3D&, const Vector3D&)>& eventSetter);

	Zephyrus::ActorComponent::Component* DrawActorComponents(Zephyrus::ActorComponent::Actor* pActor, Zephyrus::Factory::ComponentFactory* compFactory);
	bool DrawComponent(Zephyrus::ActorComponent::Component* pComponent);
	
private:
	void CreateSetLocation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetRotation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetSize(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
};
