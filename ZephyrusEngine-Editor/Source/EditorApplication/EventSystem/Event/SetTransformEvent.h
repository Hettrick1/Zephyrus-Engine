#pragma once
#include "Event.h"
#include "Vector3D.h"
#include "Quaternion.h"

class Actor;

class SetPositionEvent : public Event
{
private:
	Vector3D mNextPosition;
	Vector3D mLastPosition;
	Actor* mActor = nullptr;
public:
	SetPositionEvent(Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition);
	~SetPositionEvent();

	void Execute() override;
	void Undo() override;
};

class SetRotationEvent : public Event
{
private:
	Quaternion mNextRotation;
	Quaternion mLastRotation;
	Actor* mActor = nullptr;
public:
	SetRotationEvent(Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation);
	~SetRotationEvent();

	void Execute() override;
	void Undo() override;
};

class SetSizeEvent : public Event
{
private:
	Vector3D mNextSize;
	Vector3D mLastSize;
	Actor* mActor = nullptr;
public:
	SetSizeEvent(Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize);
	~SetSizeEvent();

	void Execute() override;
	void Undo() override;
};