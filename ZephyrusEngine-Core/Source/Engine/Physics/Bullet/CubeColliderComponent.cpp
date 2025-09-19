#include "CubeColliderComponent.h"

#include "BulletRigidbodyComponent.h"
#include "Actor.h"


CubeColliderComponent::CubeColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner)
{
    mShape = new btBoxShape(mHalfExtents);
}

void CubeColliderComponent::SetHalfExtents(const Vector3D& pHalfExtents)
{
    btVector3 newExtents = pHalfExtents.ToBulletVec3();
    if (newExtents != mHalfExtents)
    {
        mHalfExtents = newExtents;
        if (mShape)
        {
            delete mShape;
            mShape = new btBoxShape(mHalfExtents);
            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->Rebuild(mShape);
            }
        }
    }
}
