#pragma once

#include "Vector3D.h"

namespace Zephyrus::ActorComponent
{
    class BulletColliderComponent;
    class Actor;
}
namespace Zephyrus::Physics
{
    struct HitResult {
        bool HasHit = 0;
        Vector3D HitPoint = Vector3D::zero;
        Vector3D Normal = Vector3D::zero;
        Zephyrus::ActorComponent::Actor* HitActor = nullptr;
        Zephyrus::ActorComponent::BulletColliderComponent* HitCollider = nullptr;
        float Distance = 0;

        inline void Reset() {
            HasHit = 0;
            HitPoint = Vector3D::zero;
            Normal = Vector3D::zero;
            HitActor = nullptr;
            HitCollider = nullptr;
            Distance = 0;
        }
    };
}