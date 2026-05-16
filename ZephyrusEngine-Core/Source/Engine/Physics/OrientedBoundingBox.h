#pragma once

#include "Vector3D.h"
#include "Matrix4DRow.h"

namespace Zephyrus::Physics
{
    struct OrientedBoundingBox
    {
        OrientedBoundingBox(const Vector3D& pCenter, const Vector3D& pHalfExtents, const Matrix4DRow& pRotation)
            : center(pCenter), halfExtents(pHalfExtents), axisX(pRotation.GetXAxis()), axisY(pRotation.GetYAxis()), axisZ(pRotation.GetZAxis())
        {
        }

        Vector3D center;
        Vector3D halfExtents;

        Vector3D axisX;
        Vector3D axisY;
        Vector3D axisZ;

        bool IsPointInOBB(const Vector3D& point) const
        {
            Vector3D localPos(point.x - center.x, point.y - center.y, point.z - center.z);

            float localX = Vector3D::Dot(localPos, axisX);
            float localY = Vector3D::Dot(localPos, axisY);
            float localZ = Vector3D::Dot(localPos, axisZ);

            if (std::abs(localX) > halfExtents.x) return false;
            if (std::abs(localY) > halfExtents.y) return false;
            if (std::abs(localZ) > halfExtents.z) return false;

            return true;
        }
    };
}