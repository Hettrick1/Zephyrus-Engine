#include "BulletColliderComponent.h"

BulletColliderComponent::BulletColliderComponent(Actor* pOwner)
    : Component(pOwner)
{
}

BulletColliderComponent::~BulletColliderComponent()
{
    if (mShape)
    {
        delete mShape;
        mShape = nullptr;
    }
}