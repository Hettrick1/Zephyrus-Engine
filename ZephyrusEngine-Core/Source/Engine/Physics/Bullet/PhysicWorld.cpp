#include "PhysicWorld.h"
#include "Timer.h"

#include "PrefabFactory.h"
#include "BulletRigidbodyComponent.h"
#include "CubeColliderComponent.h"
#include "MeshComponent.h"

PhysicWorld::PhysicWorld()
{
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfig = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfig);
    mSolver = new btSequentialImpulseConstraintSolver();

    mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
    mWorld->setGravity(btVector3(0, 0, -9.81f));
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
    }
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
}
 
void PhysicWorld::Test()
{
    for (int i = -1; i < 2; i++)
    {
        auto actor1 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(i * 3, 10, 2));

        auto rigidbody = new BulletRigidbodyComponent(actor1);
        auto collider = new CubeColliderComponent(actor1);

        rigidbody->Initialize(collider->GetShape());

        mRigidbodies.push_back(rigidbody);

        actor1->AddComponent(rigidbody);
        actor1->AddComponent(collider);
    }
    auto actor2 = PrefabFactory::SpawnActorFromPrefab("CubeActor", Vector3D(0, 20, -3), 0, Vector3D(10, 10, 0.2));

    auto rigidbody1 = new BulletRigidbodyComponent(actor2);
    auto collider1 = new CubeColliderComponent(actor2);

    rigidbody1->Initialize(collider1->GetShape());
    rigidbody1->SetMass(0);

    mRigidbodies.push_back(rigidbody1);

    actor2->AddComponent(rigidbody1);
    actor2->AddComponent(collider1);

    collider1->SetHalfExtents(Vector3D(10, 10, 0.2));
}
