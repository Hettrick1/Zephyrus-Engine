#include "BulletColliderComponent.h"
#include "SceneManager.h"
#include "ICollisionListener.h"

BulletColliderComponent::BulletColliderComponent(Actor* pOwner)
    : Component(pOwner)
{
}

BulletColliderComponent::~BulletColliderComponent()
{
    if (mShape)
    {
        delete mShape;
        mShape = nullptr;
    }
}

void BulletColliderComponent::SetIsQuery(bool pIsQuery)
{
    if (mIsQuery == pIsQuery) return;

    mIsQuery = pIsQuery;

    auto world = SceneManager::ActiveScene->GetPhysicWorld();

    if (mIsQuery)
    {
        if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
        {
            rb->ClearRigidbody();
        }

        if (!mGhost)
        {
            mGhost = new btGhostObject();
        }
        mGhost->setUserPointer(mOwner);
        mGhost->setCollisionShape(mShape);
        mGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

        btTransform t;
        auto actorRot = mOwner->GetTransformComponent().GetRotation();
        t.setRotation(btQuaternion(actorRot.x, actorRot.y, actorRot.z, actorRot.w));
        t.setOrigin(mOwner->GetPosition().ToBulletVec3());
        mGhost->setWorldTransform(t);

        world->AddGhostObject(mGhost);
    }
    else
    {
        if (mGhost)
        {
            world->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }

        if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
        {
            rb->Rebuild(this);
        }
    }
}

void BulletColliderComponent::UpdateTrigger()
{
    if (!mIsQuery || !mGhost) return;

    std::unordered_map<const btCollisionObject*, HitResult> currentOverlaps;

    int num = mGhost->getNumOverlappingObjects();
    for (int i = 0; i < num; ++i)
    {
        const btCollisionObject* obj = mGhost->getOverlappingObject(i);

        HitResult result;
        result.HasHit = true;
        result.HitActor = static_cast<Actor*>(obj->getUserPointer());
        result.HitCollider = nullptr;
        currentOverlaps[obj] = result;

        if (mPreviousOverlaps.find(obj) == mPreviousOverlaps.end())
        {
            NotifyListenersStarted(&result);
            ZP_CORE_INFO("Enter");
        }
        else
        {
            NotifyListenersStay(&result);
            ZP_CORE_WARN("Stay");
        }
    }

    for (auto& prev : mPreviousOverlaps)
    {
        if (currentOverlaps.find(prev.first) == currentOverlaps.end())
        {
            NotifyListenersEnded(&prev.second);
            ZP_CORE_ERROR("Exit");
        }
    }

    mPreviousOverlaps = std::move(currentOverlaps);
}

void BulletColliderComponent::AddListener(ICollisionListener* pListener)
{
    mListeners.push_back(pListener);
}

void BulletColliderComponent::RemoveListener(ICollisionListener* pListener)
{
    mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), pListener), mListeners.end());
}

void BulletColliderComponent::NotifyListenersStarted(HitResult* pInfos)
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

void BulletColliderComponent::NotifyListenersStay(HitResult* pInfos)
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

void BulletColliderComponent::NotifyListenersEnded(HitResult* pInfos)
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
