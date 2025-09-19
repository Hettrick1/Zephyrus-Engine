#pragma once
#include <btBulletDynamicsCommon.h>
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

    std::vector<BulletRigidbodyComponent*> mRigidbodies;

public:
    PhysicWorld();
    ~PhysicWorld();

    void Update();
    void Unload();

    void Test();

    btDiscreteDynamicsWorld* GetWorld() { return mWorld; }
};