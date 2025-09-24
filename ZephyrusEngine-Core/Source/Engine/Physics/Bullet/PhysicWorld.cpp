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

void PhysicWorld::Update()
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
        mWorld->stepSimulation(Timer::deltaTime);
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
    if (std::find(mRigidbodies.begin(), mRigidbodies.end(), pRigidbody) == mRigidbodies.end())
    {
        mRigidbodies.push_back(pRigidbody);
    }
}

void PhysicWorld::RemoveRigidbody(BulletRigidbodyComponent* pRigidbody)
{
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
 
void PhysicWorld::Test()
{
    for (int i = -1; i < 2; i++)
    {
        auto actor1 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(i * 3, 10, 2.5));

        auto collider = new CubeColliderComponent(actor1);
        actor1->AddComponent(collider);
        auto collider0 = new CapsuleColliderComponent(actor1);
        actor1->AddComponent(collider0);
        auto rigidbody = new BulletRigidbodyComponent(actor1);
        actor1->AddComponent(rigidbody);


        rigidbody->ApplyTorqueImpulse(Vector3D(1));
    }

    auto actor3 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(0, 10, 0.5));

    auto collider3 = new CubeColliderComponent(actor3);
    actor3->AddComponent(collider3);

    collider3->SetIsQuery(true);

    auto actor4 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(-3, 10, -0.5));

    auto collider4 = new CubeColliderComponent(actor4);
    actor4->AddComponent(collider4);

    auto actor2 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(0, 10, -3), 0, Vector3D(10, 10, 0.2));

    auto rigidbody1 = new BulletRigidbodyComponent(actor2);
    actor2->AddComponent(rigidbody1);
    auto collider1 = new CubeColliderComponent(actor2);
    actor2->AddComponent(collider1);

    rigidbody1->SetType(BodyType::Static);

    collider1->SetHalfExtents(Vector3D(10, 10, 0.2));

}
