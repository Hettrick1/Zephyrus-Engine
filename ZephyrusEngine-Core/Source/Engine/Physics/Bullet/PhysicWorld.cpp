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

    bool PhysicWorld::LineTrace(const Vector3D& pStart, const Vector3D& pEnd, HitResult& pOutHit, std::vector<Zephyrus::ActorComponent::Actor*> pIgnoreActors)
    {
        struct CustomRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
            std::vector<Zephyrus::ActorComponent::Actor*> mIgnoreActors;

            CustomRayResultCallback(const btVector3& pFrom, const btVector3& pTo, std::vector<Zephyrus::ActorComponent::Actor*> pIgnoreActors)
                : btCollisionWorld::ClosestRayResultCallback(pFrom, pTo), mIgnoreActors(pIgnoreActors) {}

            virtual bool needsCollision(btBroadphaseProxy* proxy) const override
            {
                if (!btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy))
                    return false;

                btCollisionObject* obj = static_cast<btCollisionObject*>(proxy->m_clientObject);
                if (!obj) return false;

                if (std::find(mIgnoreActors.begin(), mIgnoreActors.end(), static_cast<Zephyrus::ActorComponent::Actor*>(obj->getUserPointer())) != mIgnoreActors.end())
                    return false;

                if (obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE)
                    return false;

                return true;
            }

            btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override {
                const btCollisionObject* obj = rayResult.m_collisionObject;
                if (obj && obj->getUserPointer()) {
                    Zephyrus::ActorComponent::Actor* actor = static_cast<Zephyrus::ActorComponent::Actor*>(obj->getUserPointer());
                    if (std::find(mIgnoreActors.begin(), mIgnoreActors.end(), actor) != mIgnoreActors.end()) 
                    {
                        return 1.0f;
                    }
                }
                return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
            }
        };

        pOutHit.Reset();

        CustomRayResultCallback rayCallback(btVector3(pStart.x, pStart.y, pStart.z),
            btVector3(pEnd.x, pEnd.y, pEnd.z),
            pIgnoreActors);

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

        pOutHit.Reset();

        btBoxShape boxShape(btVector3(pHalfExtents.x, pHalfExtents.y, pHalfExtents.z));

        btTransform startTrans, endTrans;
        startTrans.setIdentity();
        startTrans.setOrigin(btVector3(pStart.x, pStart.y, pStart.z));

        
        endTrans.setIdentity();
        if (pStart == pEnd)
        {
            endTrans.setOrigin(btVector3(pEnd.x, pEnd.y, pEnd.z + 0.01f));
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

    bool PhysicWorld::BoxOverlap(const Vector3D& pLocation, const Vector3D& pHalfExtents, HitResult& pOutHit, std::vector<Actor*> pIgnoreActors)
    {
        struct BoxContactCallback : public btCollisionWorld::ContactResultCallback {
            HitResult& mResult;
            const std::vector<Actor*>& mIgnoreActors;
            bool mHitFound = false;

            BoxContactCallback(HitResult& res, const std::vector<Actor*>& ignore)
                : mResult(res), mIgnoreActors(ignore) {
            }

            virtual bool needsCollision(btBroadphaseProxy* proxy) const override
            {
                if (!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
                    return false;

                btCollisionObject* obj = static_cast<btCollisionObject*>(proxy->m_clientObject);
                if (!obj) return false;

                if (std::find(mIgnoreActors.begin(), mIgnoreActors.end(), static_cast<Zephyrus::ActorComponent::Actor*>(obj->getUserPointer())) != mIgnoreActors.end())
                    return false;

                if (obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE)
                    return false;

                return true;
            }

            virtual btScalar addSingleResult(btManifoldPoint& cp,
                const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
                const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
            {
                btCollisionObject* obj = (btCollisionObject*)colObj1Wrap->getCollisionObject();
                Actor* actor = static_cast<Actor*>(obj->getUserPointer());

                for (auto* a : mIgnoreActors) if (a == actor) return 1.0f;

                mHitFound = true;
                mResult.HasHit = true;
                mResult.HitPoint = FromBtVec3(cp.getPositionWorldOnB());
                mResult.Normal = FromBtVec3(cp.m_normalWorldOnB);
                mResult.HitActor = actor;
                return 0.0f;
            }
        };

        pOutHit.Reset();

        btBoxShape boxShape(btVector3(pHalfExtents.x, pHalfExtents.y, pHalfExtents.z));
        btCollisionObject tempObj;

        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(pLocation.x, pLocation.y, pLocation.z));
        tempObj.setWorldTransform(trans);
        tempObj.setCollisionShape(&boxShape);

        BoxContactCallback callback(pOutHit, pIgnoreActors);
        mWorld->contactTest(&tempObj, callback);

        return callback.mHitFound;
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