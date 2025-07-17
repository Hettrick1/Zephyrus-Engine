#pragma once

#include "Vector3D.h"

// Axis-Aligned Bounding Box (AABB) used for spatial queries and collision detection in 3D space.
struct AABB
{
    Vector3D min;
    Vector3D max;

    AABB() : min(Vector3D(0, 0, 0)), max(Vector3D(0, 0, 0)) {}
    AABB(const Vector3D& min, const Vector3D& max) : min(min), max(max) {}

    // Checks if the given point is inside the bounding box.
    bool Contains(const Vector3D& point) const
    {
        return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z);
    }

    // Checks if this bounding box intersects with another bounding box.
    bool Intersects(const AABB& other) const
    {
        return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y &&
            min.z <= other.max.z && max.z >= other.min.z);
    }

    bool RayIntersects(const Vector3D& rayOrigin, const Vector3D& rayDir, float& outDistance) const;
};