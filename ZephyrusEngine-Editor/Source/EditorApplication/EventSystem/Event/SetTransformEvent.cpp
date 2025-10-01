#include "SetTransformEvent.h"
#include "Actor.h"
#include "Bullet/BulletRigidbodyComponent.h"

SetPositionEvent::SetPositionEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
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
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
	}
}

void SetPositionEvent::Undo()
{
	if (mActor)
	{
		mActor->SetPosition(mLastPosition);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
	}
}

SetRotationEvent::SetRotationEvent(Zephyrus::ActorComponent::Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation)
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
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
	}
}

void SetRotationEvent::Undo()
{
	if (mActor)
	{
		mActor->SetRotation(mLastRotation);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
	}
}

SetSizeEvent::SetSizeEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize)
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
