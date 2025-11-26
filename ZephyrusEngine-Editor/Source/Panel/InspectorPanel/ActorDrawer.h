#pragma once

#include "Vector3D.h"
#include <functional>

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class ActorDrawer
{
private:
	Vector3D oldActorTransform{ 0 };
	bool keepRatio = true;
	Vector3D originalSize = 0;

	const float labelWidth = 70.0f;
public:
	ActorDrawer() = default;
	~ActorDrawer() = default;

	void DrawActorInfos(Zephyrus::ActorComponent::Actor* pActor);
	void CreateSetLocation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetRotation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void CreateSetSize(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	void SetTransform(Zephyrus::ActorComponent::Actor* pActor, const std::string& label, const Vector3D& initialValue, std::function<void(const Vector3D&)> realTimeSetter, std::function<void(Zephyrus::ActorComponent::Actor*, const Vector3D&, const Vector3D&)> eventSetter);
};
