#include "ColliderComponent.h"
#include "Physics/ICollisionListener.h"
#include "Physics/PhysicManager.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Physics/ContactManifold.h"

ColliderComponent::ColliderComponent(Actor* pOwner, int pUpdateOder)
	: Component(pOwner, pUpdateOder), mIsQuerry(false), mIsActive(true)
{
	PhysicManager::Instance().RegisterCollider(pOwner, this);
	mOwner->GetScene().GetRenderer()->AddDebugCollider(this);
}

ColliderComponent::~ColliderComponent()
{
}

void ColliderComponent::Update()
{
}

bool ColliderComponent::CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut)
{
	return true;
}

std::pair<Vector3D, Vector3D> ColliderComponent::GetCollisionPosition() const
{
	return std::pair<Vector3D, Vector3D>();
}

void ColliderComponent::DebugDraw(IRenderer& renderer)
{
}

bool ColliderComponent::GetIsQuerry()
{
	return mIsQuerry;
}

void ColliderComponent::SetIsQuerry(bool isQuerry)
{
	mIsQuerry = isQuerry;
}

void ColliderComponent::AddListener(ICollisionListener* listener)
{
	mListeners.push_back(listener);
}

void ColliderComponent::RemoveListener(ICollisionListener* listenerToRemove)
{
	mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), listenerToRemove), mListeners.end());
}

void ColliderComponent::NotifyListenersStarted(HitResult* infos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerEnter(this, infos);
			}
		}
	}
}

void ColliderComponent::NotifyListenersStay(HitResult* infos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerStay(this, infos);
			}
		}
	}
}

void ColliderComponent::NotifyListenersEnded(HitResult* infos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerExit(this, infos);
			}
		}
	}
}

void ColliderComponent::SetActive(bool pActive)
{
	mIsActive = pActive;
}


