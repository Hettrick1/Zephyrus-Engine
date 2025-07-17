#include "PhysicManager.h"

#include "CollisionManager.h"
#include "CollisionResolver.h"
#include "Actor.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"

PhysicManager& PhysicManager::Instance()
{
    static PhysicManager instance;
    return instance;
}

PhysicManager::PhysicManager()
{
    mCollisionResolver = new CollisionResolver();
    mCollisionManager = new CollisionManager();
}


PhysicManager::~PhysicManager()
{
    Unload();
    if (mCollisionManager != nullptr)
    {
        delete mCollisionManager;
        mCollisionManager = nullptr;
    }
    if (mCollisionResolver != nullptr)
    {
        delete mCollisionResolver;
        mCollisionResolver = nullptr;
    }
}

void PhysicManager::Update()
{
    mCollisionResolver->UpdateRigidbodies();
    mCollisionManager->UpdateColliders();
    mCollisionManager->CheckCollisions();
    mCollisionResolver->ResolveCollisions();
    mCollisionManager->UpdateColliders();
}

void PhysicManager::Unload()
{
    mCollisionManager->Unload();
    mCollisionResolver->Unload();
}

void PhysicManager::RegisterCollider(Actor* pOwner, ColliderComponent* pCollider)
{
    mCollisionManager->RegisterCollider(pOwner, pCollider);
}

void PhysicManager::RemoveCollider(Actor* pOwner, ColliderComponent* pCollider)
{
    mCollisionManager->RemoveCollider(pOwner, pCollider);
}

bool PhysicManager::LineTrace(const Vector3D& start, const Vector3D& end, HitResult& outHit, Actor* ignoreActor)
{
    return mCollisionManager->LineTrace(start, end, outHit, ignoreActor);
}

void PhysicManager::RegisterRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody)
{
    mCollisionResolver->RegisterRigidBody(pOwner, pRigidbody);
}

void PhysicManager::RemoveRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody)
{
    mCollisionResolver->RemoveRigidBody(pOwner, pRigidbody);
}

void PhysicManager::AddCollisionToQueue(CollisionInfos* pCollisionInfo)
{
    mCollisionResolver->AddCollisionToQueue(pCollisionInfo);
}
