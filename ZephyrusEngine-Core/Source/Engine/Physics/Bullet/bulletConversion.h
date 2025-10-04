#pragma once

#include "Vector3D.h"
#include "Quaternion.h"
#include "btBulletDynamicsCommon.h"

namespace Zephyrus::Physics
{
    inline btVector3 ToBtVec3(const Vector3D& v) {
        return btVector3{ v.x, v.y, v.z };
    }

    inline Vector3D FromBtVec3(const btVector3& v) {
        return Vector3D{ v.x(), v.y(), v.z() };
    }

    inline btQuaternion ToBtQuat(const Quaternion& q) {
        return btQuaternion{ q.x, q.y, q.z, q.w };
    }

    inline Quaternion FromBtQuat(const btQuaternion& q) {
        return Quaternion{ q.x(), q.y(), q.z(), q.w()};
    }
}