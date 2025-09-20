#pragma once
#include "BulletColliderComponent.h"

class SphereColliderComponent : public BulletColliderComponent
{
private:
    float mRadius = 1;
public:
    SphereColliderComponent(Actor* pOwner);

    void SetRadius(const float& pRadius);
};
