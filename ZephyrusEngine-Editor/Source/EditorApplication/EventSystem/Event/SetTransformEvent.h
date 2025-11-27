#pragma once
#include "Event.h"
#include "Vector3D.h"
#include "Quaternion.h"

namespace Zephyrus::ActorComponent
{
	class Component;
}

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class SetActorPositionEvent : public Event
{
private:
	Vector3D mNextPosition;
	Vector3D mLastPosition;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetActorPositionEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	~SetActorPositionEvent() override;

	void Execute() override;
	void Undo() override;
};

class SetActorRotationEvent : public Event
{
private:
	Quaternion mNextRotation;
	Quaternion mLastRotation;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetActorRotationEvent(Zephyrus::ActorComponent::Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation);
	~SetActorRotationEvent() override;

	void Execute() override;
	void Undo() override;
};

class SetActorSizeEvent : public Event
{
private:
	Vector3D mNextSize;
	Vector3D mLastSize;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetActorSizeEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize);
	~SetActorSizeEvent() override;

	void Execute() override;
	void Undo() override;
};

class SetComponentPositionEvent : public Event
{
private:
	Vector3D mNextPosition;
	Vector3D mLastPosition;
	Zephyrus::ActorComponent::Component* mComponent{ nullptr };
public:
	SetComponentPositionEvent(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	~SetComponentPositionEvent() override;

	void Execute() override;
	void Undo() override;
};

class SetComponentRotationEvent : public Event
{
private:
	Quaternion mNextRotation;
	Quaternion mLastRotation;
	Zephyrus::ActorComponent::Component* mComponent{ nullptr };
public:
	SetComponentRotationEvent(Zephyrus::ActorComponent::Component* pComponent, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation);
	~SetComponentRotationEvent() override;

	void Execute() override;
	void Undo() override;
};

class SetComponentSizeEvent : public Event
{
private:
	Vector3D mNextSize;
	Vector3D mLastSize;
	Zephyrus::ActorComponent::Component* mComponent{ nullptr };
public:
	SetComponentSizeEvent(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentSize, const Vector3D& pNextSize);
	~SetComponentSizeEvent() override;

	void Execute() override;
	void Undo() override;
};