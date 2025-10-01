#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BulletRigidbodyComponent.h"

#include <vector>

struct CustomRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
    Zephyrus::ActorComponent::Actor* mIgnoreActor;

    CustomRayResultCallback(const btVector3& pFrom, const btVector3& pTo, Zephyrus::ActorComponent::Actor* pIgnore)
        : btCollisionWorld::ClosestRayResultCallback(pFrom, pTo), mIgnoreActor(pIgnore) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy) const override
    {
        if (!btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy))
            return false;

        btCollisionObject* obj = static_cast<btCollisionObject*>(proxy->m_clientObject);
        if (!obj) return false;

        if (obj->getUserPointer() == mIgnoreActor)
            return false;

        if (obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE)
            return false;

        return true;
    }

    btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override {
        const btCollisionObject* obj = rayResult.m_collisionObject;
        if (obj && obj->getUserPointer()) {
            Zephyrus::ActorComponent::Actor* actor = static_cast<Zephyrus::ActorComponent::Actor*>(obj->getUserPointer());
            if (actor == mIgnoreActor) {
                return 1.0f;
            }
        }
        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }
};

class PhysicWorld
{
private:
    btBroadphaseInterface* mBroadphase = nullptr;
    btDefaultCollisionConfiguration* mCollisionConfig = nullptr;
    btCollisionDispatcher* mDispatcher = nullptr;
    btSequentialImpulseConstraintSolver* mSolver = nullptr;
    btDiscreteDynamicsWorld* mWorld = nullptr;
    btGhostPairCallback* mGhostCallback = nullptr;

    std::vector<Zephyrus::ActorComponent::BulletRigidbodyComponent*> mRigidbodies;
    std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> mColliders;

public:
    PhysicWorld();
    ~PhysicWorld();

    void Update(float pDetltaTime);
    void Unload();

    void AddGhostObject(btGhostObject* ghost);
    void RemoveGhostObject(btGhostObject* ghost);
    void AddRigidbody(Zephyrus::ActorComponent::BulletRigidbodyComponent* pRigidbody);
    void RemoveRigidbody(Zephyrus::ActorComponent::BulletRigidbodyComponent* pRigidbody);
    void AddCollider(Zephyrus::ActorComponent::BulletColliderComponent* pCollider);
    void RemoveCollider(Zephyrus::ActorComponent::BulletColliderComponent* pCollider);

    bool LineTrace(const Vector3D& pStart,const Vector3D& pEnd,HitResult& pOutHit, Zephyrus::ActorComponent::Actor* pIgnoreActor = nullptr);

    btDiscreteDynamicsWorld* GetWorld() { return mWorld; }
};