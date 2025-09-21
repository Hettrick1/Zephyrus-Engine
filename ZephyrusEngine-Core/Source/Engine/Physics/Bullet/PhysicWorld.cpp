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

        auto rigidbody = new BulletRigidbodyComponent(actor1);
        auto collider = new CubeColliderComponent(actor1);

        rigidbody->Initialize(collider);

        mRigidbodies.push_back(rigidbody);
        mColliders.push_back(collider);

        actor1->AddComponent(rigidbody);
        actor1->AddComponent(collider);
    }

    auto actor3 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(0, 10, 0.5));

    auto rigidbody3 = new BulletRigidbodyComponent(actor3);
    auto collider3 = new CubeColliderComponent(actor3);

    rigidbody3->Initialize(collider3);

    mRigidbodies.push_back(rigidbody3);
    mColliders.push_back(collider3);

    actor3->AddComponent(rigidbody3);
    actor3->AddComponent(collider3);

    mColliders[3]->SetIsQuery(true);



    auto actor2 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(0, 10, -3), 0, Vector3D(10, 10, 0.2));

    auto rigidbody1 = new BulletRigidbodyComponent(actor2);
    auto collider1 = new CubeColliderComponent(actor2);

    rigidbody1->Initialize(collider1);
    rigidbody1->SetMass(0);

    mRigidbodies.push_back(rigidbody1);

    actor2->AddComponent(rigidbody1);
    actor2->AddComponent(collider1);

    collider1->SetHalfExtents(Vector3D(10, 10, 0.2));
}
