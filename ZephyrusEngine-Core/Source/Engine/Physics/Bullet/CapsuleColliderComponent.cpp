#include "CapsuleColliderComponent.h"
#include "Actor.h"
#include "BulletRigidbodyComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor* pOwner)
	: BulletColliderComponent(pOwner)
{
	mShape = new btCapsuleShape(mRadius, mHeight);
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
            delete mShape;
            mShape = new btCapsuleShape(mRadius, mHeight);
            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->Rebuild(this);
            }
        }
    }
}
