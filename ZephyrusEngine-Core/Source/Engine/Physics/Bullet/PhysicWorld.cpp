#include "PhysicWorld.h"
#include "Timer.h"

#include "PrefabFactory.h"
#include "BulletRigidbodyComponent.h"
#include "CubeColliderComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "MeshComponent.h"

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
    if (mWorld)
    {
        for (auto& rigidbody : mRigidbodies)
        {
            rigidbody->SyncTransformFromWorld();
        }
        for (auto& collider : mColliders)
        {
            collider->UpdateWorldTransform();
        }
        mWorld->stepSimulation(pDetltaTime);
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

void PhysicWorld::AddRigidbody(Zephyrus::ActorComponent::BulletRigidbodyComponent* pRigidbody)
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

void PhysicWorld::RemoveRigidbody(Zephyrus::ActorComponent::BulletRigidbodyComponent* pRigidbody)
{
    if (!pRigidbody) return;

    auto rb = pRigidbody->GetRigidBody();
    if (rb)
    {
        mWorld->removeRigidBody(rb);
    }
    std::erase(mRigidbodies, pRigidbody);
}

void PhysicWorld::AddCollider(Zephyrus::ActorComponent::BulletColliderComponent* pCollider)
{
    if (std::find(mColliders.begin(), mColliders.end(), pCollider) == mColliders.end())
    {
        mColliders.push_back(pCollider);
    }
}
void PhysicWorld::RemoveCollider(Zephyrus::ActorComponent::BulletColliderComponent* pCollider)
{
    std::erase(mColliders, pCollider);
}

bool PhysicWorld::LineTrace(const Vector3D& pStart, const Vector3D& pEnd, HitResult& pOutHit, Zephyrus::ActorComponent::Actor* pIgnoreActor)
{
    pOutHit.Reset();

    CustomRayResultCallback rayCallback(btVector3(pStart.x, pStart.y, pStart.z),
        btVector3(pEnd.x, pEnd.y, pEnd.z),
        pIgnoreActor);

    mWorld->rayTest(rayCallback.m_rayFromWorld, rayCallback.m_rayToWorld, rayCallback);

    if (rayCallback.hasHit()) {
        pOutHit.HasHit = true;
        pOutHit.HitPoint = Vector3D(rayCallback.m_hitPointWorld);
        pOutHit.Normal = Vector3D(rayCallback.m_hitNormalWorld);

        const btCollisionObject* hitObj = rayCallback.m_collisionObject;
        if (hitObj && hitObj->getUserPointer()) {
            pOutHit.HitActor = static_cast<Zephyrus::ActorComponent::Actor*>(hitObj->getUserPointer());
            pOutHit.HitCollider = nullptr;
        }

        pOutHit.Distance = (pOutHit.HitPoint - pStart).Length();
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
