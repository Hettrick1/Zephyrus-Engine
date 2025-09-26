#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BulletRigidbodyComponent.h"

#include <vector>

struct CustomRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
    Actor* mIgnoreActor;

    CustomRayResultCallback(const btVector3& pFrom, const btVector3& pTo, Actor* pIgnore)
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
            Actor* actor = static_cast<Actor*>(obj->getUserPointer());
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

    std::vector<BulletRigidbodyComponent*> mRigidbodies;
    std::vector<BulletColliderComponent*> mColliders;

public:
    PhysicWorld();
    ~PhysicWorld();

    void Update();
    void Unload();

    void AddGhostObject(btGhostObject* ghost);
    void RemoveGhostObject(btGhostObject* ghost);
    void AddRigidbody(BulletRigidbodyComponent* pRigidbody);
    void RemoveRigidbody(BulletRigidbodyComponent* pRigidbody);
    void AddCollider(BulletColliderComponent* pCollider);
    void RemoveCollider(BulletColliderComponent* pCollider);

    bool LineTrace(const Vector3D& pStart,const Vector3D& pEnd,HitResult& pOutHit,Actor* pIgnoreActor = nullptr);

    btDiscreteDynamicsWorld* GetWorld() { return mWorld; }
};