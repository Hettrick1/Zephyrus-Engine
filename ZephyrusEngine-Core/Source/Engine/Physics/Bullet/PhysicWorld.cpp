#include "pch.h"
#include "PhysicWorld.h"
#include "Timer.h"

#include "PrefabFactory.h"
#include "BulletRigidbodyComponent.h"
#include "CubeColliderComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "MeshComponent.h"

namespace Zephyrus::Physics
{
    struct CustomConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
        std::vector<Zephyrus::ActorComponent::Actor*> mIgnoreActors;

        CustomConvexResultCallback(const btVector3& pFrom, const btVector3& pTo, const std::vector<Zephyrus::ActorComponent::Actor*>& pIgnore)
            : btCollisionWorld::ClosestConvexResultCallback(pFrom, pTo), mIgnoreActors(pIgnore) {}

        virtual bool needsCollision(btBroadphaseProxy* proxy) const override {
            if (!btCollisionWorld::ClosestConvexResultCallback::needsCollision(proxy))
                return false;

            btCollisionObject* obj = static_cast<btCollisionObject*>(proxy->m_clientObject);
            if (!obj) return false;

            auto* actor = static_cast<Zephyrus::ActorComponent::Actor*>(obj->getUserPointer());
            for (auto* ignore : mIgnoreActors) {
                if (actor == ignore) return false;
            }

            return !(obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }
    };

    PhysicWorld::PhysicWorld()
    {
        mBroadphase = new btDbvtBroadphase();
        mCollisionConfig = new btDefaultCollisionConfiguration();
        mDispatcher = new btCollisionDispatcher(mCollisionConfig);
        mSolver = new btSequentialImpulseConstraintSolver();

        mGhostCallback = new btGhostPairCallback();

        mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
        mWorld->setGravity(btVector3(0, 0, -9.81f));
        mWorld->getPairCache()->setInternalGhostPairCallback(mGhostCallback);
    }

    PhysicWorld::~PhysicWorld()
    {
        Unload();
    }

    void PhysicWorld::Update(float pDetltaTime)
    {
        if (mWorld && pDetltaTime > 0.000001f)
        {
            for (auto& rigidbody : mRigidbodies)
            {
                rigidbody->SyncTransformFromWorld();
            }
            for (auto& collider : mColliders)
            {
                collider->UpdateWorldTransform();
            }
            mWorld->stepSimulation(pDetltaTime, 10);
            for (auto& rigidbody : mRigidbodies)
            {
                rigidbody->SyncTransformFromPhysics();
            }
            for (auto& collider : mColliders)
            {
                collider->UpdateTrigger();
            }
        }
    }

    void PhysicWorld::AddGhostObject(btGhostObject* ghost)
    {
        mWorld->addCollisionObject(
            ghost,
            btBroadphaseProxy::SensorTrigger,
            btBroadphaseProxy::AllFilter
        );
    }

    void PhysicWorld::RemoveGhostObject(btGhostObject* ghost)
    {
        mWorld->removeCollisionObject(ghost);
    }

    void PhysicWorld::AddRigidbody(BulletRigidbodyComponent* pRigidbody)
    {
        if (!pRigidbody) return;

        auto rb = pRigidbody->GetRigidBody();
        if (!rb) return;

        if (std::find(mRigidbodies.begin(), mRigidbodies.end(), pRigidbody) == mRigidbodies.end())
        {
            mWorld->addRigidBody(rb);
            mRigidbodies.push_back(pRigidbody);
        }
    }

    void PhysicWorld::RemoveRigidbody(BulletRigidbodyComponent* pRigidbody)
    {
        if (!pRigidbody) return;

        auto rb = pRigidbody->GetRigidBody();
        if (rb)
        {
            mWorld->removeRigidBody(rb);
        }
        std::erase(mRigidbodies, pRigidbody);
    }

    void PhysicWorld::AddCollider(BulletColliderComponent* pCollider)
    {
        if (std::find(mColliders.begin(), mColliders.end(), pCollider) == mColliders.end())
        {
            mColliders.push_back(pCollider);
        }
    }
    void PhysicWorld::RemoveCollider(BulletColliderComponent* pCollider)
    {
        std::erase(mColliders, pCollider);
    }

    bool PhysicWorld::LineTrace(const Vector3D& pStart, const Vector3D& pEnd, HitResult& pOutHit, Actor* pIgnoreActor)
    {
        pOutHit.Reset();

        CustomRayResultCallback rayCallback(btVector3(pStart.x, pStart.y, pStart.z),
            btVector3(pEnd.x, pEnd.y, pEnd.z),
            pIgnoreActor);

        mWorld->rayTest(rayCallback.m_rayFromWorld, rayCallback.m_rayToWorld, rayCallback);

        if (rayCallback.hasHit()) {
            pOutHit.HasHit = true;
            pOutHit.HitPoint = Zephyrus::Physics::FromBtVec3(rayCallback.m_hitPointWorld);
            pOutHit.Normal = Zephyrus::Physics::FromBtVec3(rayCallback.m_hitNormalWorld);

            const btCollisionObject* hitObj = rayCallback.m_collisionObject;
            if (hitObj && hitObj->getUserPointer()) {
                pOutHit.HitActor = static_cast<Actor*>(hitObj->getUserPointer());
                pOutHit.HitCollider = nullptr;
            }

            pOutHit.Distance = (pOutHit.HitPoint - pStart).Length();
            return true;
        }
        return false;
    }

    bool PhysicWorld::BoxTrace(const Vector3D& pStart, const Vector3D& pEnd, const Vector3D& pHalfExtents, HitResult& pOutHit, std::vector<Actor*> pIgnoreActors)
    {
        pOutHit.Reset();

        btBoxShape boxShape(btVector3(pHalfExtents.x, pHalfExtents.y, pHalfExtents.z));

        btTransform startTrans, endTrans;
        startTrans.setIdentity();
        startTrans.setOrigin(btVector3(pStart.x, pStart.y, pStart.z));

        
        endTrans.setIdentity();
        if (pStart == pEnd)
        {
            endTrans.setOrigin(btVector3(pEnd.x + 0.01f, pEnd.y + 0.01f, pEnd.z + 0.01f));
        }
        else
        {
            endTrans.setOrigin(btVector3(pEnd.x, pEnd.y, pEnd.z));
        }

        CustomConvexResultCallback callback(startTrans.getOrigin(), endTrans.getOrigin(), pIgnoreActors);

        callback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
        callback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;

        mWorld->convexSweepTest(&boxShape, startTrans, endTrans, callback, mWorld->getDispatchInfo().m_allowedCcdPenetration);

        if (callback.hasHit()) {
            pOutHit.HasHit = true;
            pOutHit.HitPoint = Zephyrus::Physics::FromBtVec3(callback.m_hitPointWorld);
            pOutHit.Normal = Zephyrus::Physics::FromBtVec3(callback.m_hitNormalWorld);

            if (callback.m_hitCollisionObject && callback.m_hitCollisionObject->getUserPointer()) {
                pOutHit.HitActor = static_cast<Actor*>(callback.m_hitCollisionObject->getUserPointer());
            }

            float totalDistance = (pEnd - pStart).Length();
            pOutHit.Distance = callback.m_closestHitFraction * totalDistance;
            return true;
        }

        return false;
    }

    void PhysicWorld::Unload()
    {
        delete mWorld;
        mWorld = nullptr;
        delete mSolver;
        mSolver = nullptr;
        delete mDispatcher;
        mDispatcher = nullptr;
        delete mCollisionConfig;
        mCollisionConfig = nullptr;
        delete mBroadphase;
        mBroadphase = nullptr;
        delete mGhostCallback;
        mGhostCallback = nullptr;
    }
}