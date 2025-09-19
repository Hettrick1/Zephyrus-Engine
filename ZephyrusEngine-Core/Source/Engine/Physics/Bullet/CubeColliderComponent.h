#pragma once
#include "BulletColliderComponent.h"

class CubeColliderComponent : public BulletColliderComponent
{
private:
    btVector3 mHalfExtents = btVector3(1, 1, 1);
public:
    CubeColliderComponent(Actor* pOwner);

    void SetHalfExtents(const Vector3D& pHalfExtents);
};
