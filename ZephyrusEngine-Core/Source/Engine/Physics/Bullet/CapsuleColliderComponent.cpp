#include "CapsuleColliderComponent.h"
#include "Actor.h"
#include "BulletRigidbodyComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor* pOwner)
	: BulletColliderComponent(pOwner)
{
	mShape = new btCapsuleShapeZ(mRadius, mHeight);
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
    else
    {
        CreateColliderWithoutBody();
    }
}

void CapsuleColliderComponent::SetRadiusAndHeight(const float& pRadius, const float& pHeight)
{
    float newRadius = pRadius;
    float newHeight = pHeight;
    if (newRadius != mRadius || newHeight != mHeight)
    {
        mRadius = newRadius;
        mHeight = newHeight;
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btCapsuleShapeZ(mRadius, mHeight);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
