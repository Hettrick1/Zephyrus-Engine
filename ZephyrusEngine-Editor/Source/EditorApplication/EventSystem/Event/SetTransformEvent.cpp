#include "pch.h"
#include "SetTransformEvent.h"
#include "Actor.h"
#include "Bullet/BulletRigidbodyComponent.h"

SetActorPositionEvent::SetActorPositionEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
	: Event("Move Actor"), mActor(pActor), mNextPosition(pNextPosition), mLastPosition(pCurrentPosition)
{
}

SetActorPositionEvent::~SetActorPositionEvent()
{
	mActor = nullptr;
}

void SetActorPositionEvent::Execute()
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
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

void SetActorPositionEvent::Undo()
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
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

SetActorRotationEvent::SetActorRotationEvent(Zephyrus::ActorComponent::Actor* pActor, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation)
	: Event("Rotate Actor"), mActor(pActor), mNextRotation(pNextRotation), mLastRotation(pCurrentRotation)
{
}

SetActorRotationEvent::~SetActorRotationEvent()
{
	mActor = nullptr;
}

void SetActorRotationEvent::Execute()
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
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

void SetActorRotationEvent::Undo()
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
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

SetActorSizeEvent::SetActorSizeEvent(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentSize, const Vector3D& pNextSize)
	: Event("Resize Actor"), mActor(pActor), mNextSize(pNextSize), mLastSize(pCurrentSize)
{
}

SetActorSizeEvent::~SetActorSizeEvent()
{
	mActor = nullptr;
}

void SetActorSizeEvent::Execute()
{
	if (mActor)
	{
		mActor->SetSize(mNextSize);
	}
}

void SetActorSizeEvent::Undo()
{
	if (mActor)
	{
		mActor->SetSize(mLastSize);
	}
}


// COMPONENT POSITION
SetComponentPositionEvent::SetComponentPositionEvent(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
	: Event("Move Component"), mComponent(pComponent), mNextPosition(pNextPosition), mLastPosition(pCurrentPosition)
{
}

SetComponentPositionEvent::~SetComponentPositionEvent()
{
	mComponent = nullptr;
}

void SetComponentPositionEvent::Execute()
{
	if (mComponent)
	{
		mComponent->SetRelativePosition(mNextPosition);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

void SetComponentPositionEvent::Undo()
{
	if (mComponent)
	{
		mComponent->SetRelativePosition(mLastPosition);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}


// COMPONENT ROTATION
SetComponentRotationEvent::SetComponentRotationEvent(Zephyrus::ActorComponent::Component* pComponent, const Quaternion& pCurrentRotation, const Quaternion& pNextRotation)
	: Event("Rotate Component"), mComponent(pComponent), mNextRotation(pNextRotation), mLastRotation(pCurrentRotation)
{
}

SetComponentRotationEvent::~SetComponentRotationEvent()
{
	mComponent = nullptr;
}

void SetComponentRotationEvent::Execute()
{
	if (mComponent)
	{
		mComponent->SetRelativeRotation(mNextRotation);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

void SetComponentRotationEvent::Undo()
{
	if (mComponent)
	{
		mComponent->SetRelativeRotation(mLastRotation);
		std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> rigidbodies = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletRigidbodyComponent>();
		if (!rigidbodies.empty())
		{
			for (auto rb : rigidbodies)
			{
				rb->ForceSyncFromActor();
			}
		}
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = mComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
}

// COMPONENT SIZE
SetComponentSizeEvent::SetComponentSizeEvent(Zephyrus::ActorComponent::Component* pComponent, const Vector3D& pCurrentSize, const Vector3D& pNextSize)
	: Event("Resize Component"), mComponent(pComponent), mNextSize(pNextSize), mLastSize(pCurrentSize)
{
}

SetComponentSizeEvent::~SetComponentSizeEvent()
{
	mComponent = nullptr;
}

void SetComponentSizeEvent::Execute()
{
	if (mComponent)
	{
		mComponent->SetRelativeSize(mNextSize);
	}
}

void SetComponentSizeEvent::Undo()
{
	if (mComponent)
	{
		mComponent->SetRelativeSize(mLastSize);
	}
}

