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
	Vector3D oldActorTransform{ 0 };
	bool keepRatio{ true };
	Vector3D originalSize{ 0 };

	float labelWidth{ 80.0f };

	unsigned int selected{ 0 };
	bool mSelfSelected{ true };
	Zephyrus::ActorComponent::Component* activeComponent{nullptr};
public:
	ActorDrawer() = default;
	~ActorDrawer() = default;

	void DrawActorInfos(Zephyrus::ActorComponent::Actor* pActor);
	void SetTransform(Zephyrus::ActorComponent::Actor* pActor, const std::string& label, float step, const Vector3D& initialValue, const std::function<void(const Vector3D&)>& realTimeSetter, const std::function<void(Zephyrus::ActorComponent::Actor*, const Vector3D&, const Vector3D&)>& eventSetter);

	Zephyrus::ActorComponent::Component* DrawActorComponents(Zephyrus::ActorComponent::Actor* pActor, Zephyrus::Factory::ComponentFactory* compFactory);
	
private:
	void CreateSetLocation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetRotation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetSize(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
};
