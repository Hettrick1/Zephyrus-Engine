#include "SetTransformEvent.h"
#include "Actor.h"

SetPositionEvent::SetPositionEvent(Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
	: Event("Move Actor"), mActor(pActor), mNextPosition(pNextPosition), mLastPosition(pCurrentPosition)
{
}

SetPositionEvent::~SetPositionEvent()
{
	mActor = nullptr;
}

void SetPositionEvent::Execute()
{
	if (mActor)
	{
		mActor->SetPosition(mNextPosition);
	}
}

void SetPositionEvent::Undo()
{
	if (mActor)
	{
		mActor->SetPosition(mLastPosition);
	}
}

SetRotationEvent::SetRotationEvent(Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation)
	: Event("Rotate Actor"), mActor(pActor), mNextRotation(pNextRotation), mLastRotation(pCurrentRotation)
{
}

SetRotationEvent::~SetRotationEvent()
{
	mActor = nullptr;
}

void SetRotationEvent::Execute()
{
	if (mActor)
	{
		mActor->SetRotation(mNextRotation);
	}
}

void SetRotationEvent::Undo()
{
	if (mActor)
	{
		mActor->SetRotation(mLastRotation);
	}
}

SetSizeEvent::SetSizeEvent(Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize)
	: Event("Resize Actor"), mActor(pActor), mNextSize(pNextSize), mLastSize(pCurrentSize)
{
}

SetSizeEvent::~SetSizeEvent()
{
	mActor = nullptr;
}

void SetSizeEvent::Execute()
{
	if (mActor)
	{
		mActor->SetSize(mNextSize);
	}
}

void SetSizeEvent::Undo()
{
	if (mActor)
	{
		mActor->SetSize(mLastSize);
	}
}
