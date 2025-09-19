#pragma once
#include "Component.h"

#include <btBulletDynamicsCommon.h>

class BulletColliderComponent : public Component
{
public:
    BulletColliderComponent(Actor* pOwner);
    virtual ~BulletColliderComponent();

    btCollisionShape* GetShape() const { return mShape; }

protected:
    btCollisionShape* mShape = nullptr;
};