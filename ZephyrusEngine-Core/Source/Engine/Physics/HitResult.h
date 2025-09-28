#pragma once

#include "Vector3D.h"

class BulletColliderComponent;
class Actor;

struct HitResult {
    bool HasHit = 0;          
    Vector3D HitPoint = Vector3D::zero;  
    Vector3D Normal = Vector3D::zero;
    Actor* HitActor = nullptr;
    BulletColliderComponent* HitCollider = nullptr;
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
