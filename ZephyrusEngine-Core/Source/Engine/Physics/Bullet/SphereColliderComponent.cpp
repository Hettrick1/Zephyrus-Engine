#include "SphereColliderComponent.h"
#include "Actor.h"
#include "BulletRigidbodyComponent.h"

SphereColliderComponent::SphereColliderComponent(Actor* pOwner)
	: BulletColliderComponent(pOwner)
{
	mShape = new btSphereShape(mRadius);
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
}

void SphereColliderComponent::SetRadius(const float& pRadius)
{
    float newRadius = pRadius;
    if (newRadius != mRadius)
    {
        mRadius = newRadius;
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btSphereShape(mRadius);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
