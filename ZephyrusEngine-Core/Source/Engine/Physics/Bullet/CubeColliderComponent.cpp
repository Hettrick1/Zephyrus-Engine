#include "CubeColliderComponent.h"

#include "BulletRigidbodyComponent.h"
#include "Actor.h"
#include "SceneManager.h"

CubeColliderComponent::CubeColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner)
{
    mShape = new btBoxShape(mHalfExtents);
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
    else
    {
        CreateColliderWithoutBody();
    }
}

void CubeColliderComponent::SetHalfExtents(const Vector3D& pHalfExtents)
{
    btVector3 newExtents = pHalfExtents.ToBulletVec3();
    if (newExtents != mHalfExtents)
    {
        mHalfExtents = newExtents;
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btBoxShape(mHalfExtents);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
