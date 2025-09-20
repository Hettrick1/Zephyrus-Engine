#pragma once
#include "Component.h"

#include <btBulletDynamicsCommon.h>

class BulletColliderComponent : public Component
{
public:
    BulletColliderComponent(Actor* pOwner);
    virtual ~BulletColliderComponent();

    btCollisionShape* GetShape() const { return mShape; }

    inline bool GetIsQuery() const { return mIsQuery; }

protected:
    btCollisionShape* mShape = nullptr;
    bool mIsQuery = false;
};