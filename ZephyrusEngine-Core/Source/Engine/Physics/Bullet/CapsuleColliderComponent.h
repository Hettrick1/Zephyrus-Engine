#pragma once
#include "BulletColliderComponent.h"

class CapsuleColliderComponent : public BulletColliderComponent
{
private:
    float mRadius = 1;
    float mHeight = 2;
public:
    CapsuleColliderComponent(Actor* pOwner);

    void SetRadiusAndHeight(const float& pRadius = 1, const float& pHeight = 2);
};