#include "ColliderComponent.h"
#include "Physics/ICollisionListener.h"
#include "Physics/PhysicManager.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Physics/ContactManifold.h"

ColliderComponent::ColliderComponent(Actor* pOwner, const std::string& pName ,int pUpdateOder)
	: Component(pOwner, pName, pUpdateOder)
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

void ColliderComponent::OnEnd()
{
	PhysicManager::Instance().RemoveCollider(mOwner, this);
	mOwner->GetScene().GetRenderer()->RemoveDebugCollider(this);
}

bool ColliderComponent::CheckCollisionWith(ColliderComponent* pOther, ContactManifold& pInfosOut)
{
	return true;
}

std::pair<Vector3D, Vector3D> ColliderComponent::GetCollisionPosition() const
{
	return std::pair<Vector3D, Vector3D>();
}

void ColliderComponent::DebugDraw(IRenderer& pRenderer)
{
}

bool ColliderComponent::GetIsQuerry()
{
	return mIsQuerry;
}

void ColliderComponent::SetIsQuerry(bool pIsQuerry)
{
	mIsQuerry = pIsQuerry;
}

void ColliderComponent::AddListener(ICollisionListener* pListener)
{
	mListeners.push_back(pListener);
}

void ColliderComponent::RemoveListener(ICollisionListener* pListener)
{
	mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), pListener), mListeners.end());
}

void ColliderComponent::NotifyListenersStarted(HitResult* pInfos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerEnter(this, pInfos);
			}
		}
	}
}

void ColliderComponent::NotifyListenersStay(HitResult* pInfos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerStay(this, pInfos);
			}
		}
	}
}

void ColliderComponent::NotifyListenersEnded(HitResult* pInfos)
{
	if (mListeners.size() > 0) {
		for (ICollisionListener* listener : mListeners) {
			if (listener != nullptr)
			{
				listener->OnTriggerExit(this, pInfos);
			}
		}
	}
}

void ColliderComponent::SetActive(bool pActive)
{
	mIsActive = pActive;
}


