#pragma once
#include "Event.h"
#include "Vector3D.h"
#include "Quaternion.h"

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class SetPositionEvent : public Event
{
private:
	Vector3D mNextPosition;
	Vector3D mLastPosition;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetPositionEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	~SetPositionEvent();

	void Execute() override;
	void Undo() override;
};

class SetRotationEvent : public Event
{
private:
	Quaternion mNextRotation;
	Quaternion mLastRotation;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetRotationEvent(Zephyrus::ActorComponent::Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation);
	~SetRotationEvent();

	void Execute() override;
	void Undo() override;
};

class SetSizeEvent : public Event
{
private:
	Vector3D mNextSize;
	Vector3D mLastSize;
	Zephyrus::ActorComponent::Actor* mActor = nullptr;
public:
	SetSizeEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize);
	~SetSizeEvent();

	void Execute() override;
	void Undo() override;
};