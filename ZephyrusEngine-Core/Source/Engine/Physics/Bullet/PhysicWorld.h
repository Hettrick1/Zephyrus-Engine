#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BulletRigidbodyComponent.h"

#include <vector>
namespace Zephyrus::Physics
{
    using Zephyrus::ActorComponent::BulletRigidbodyComponent;
    using Zephyrus::ActorComponent::BulletColliderComponent;
    using Zephyrus::ActorComponent::Actor;

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

        void Update(float pDetltaTime);
        void Unload();

        void AddGhostObject(btGhostObject* ghost);
        void RemoveGhostObject(btGhostObject* ghost);
        void AddRigidbody(BulletRigidbodyComponent* pRigidbody);
        void RemoveRigidbody(BulletRigidbodyComponent* pRigidbody);
        void AddCollider(BulletColliderComponent* pCollider);
        void RemoveCollider(BulletColliderComponent* pCollider);

        bool LineTrace(const Vector3D& pStart, const Vector3D& pEnd, HitResult& pOutHit, std::vector<Actor*> pIgnoreActors = {});
        bool BoxTrace(const Vector3D& pStart, const Vector3D& pEnd, const Vector3D& pExtents, HitResult& pOutHit, std::vector<Actor*> pIgnoreActors = {});
        bool BoxOverlap(const Vector3D& pLocation, const Vector3D& pHalfExtents, HitResult& pOutHit, std::vector<Actor*> pIgnoreActors = {});

        btDiscreteDynamicsWorld* GetWorld() { return mWorld; }
    };
}