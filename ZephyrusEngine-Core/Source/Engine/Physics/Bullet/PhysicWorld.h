#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BulletRigidbodyComponent.h"

#include <vector>

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

    void Test();

    btDiscreteDynamicsWorld* GetWorld() { return mWorld; }
};