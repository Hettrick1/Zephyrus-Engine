#include "AABB.h"

bool AABB::RayIntersects(const Vector3D& rayOrigin, const Vector3D& rayEnd, float& outDistance) const
{
    Vector3D rayDir = rayEnd - rayOrigin;
    Vector3D invDir = { 1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z };

    Vector3D tMinVec = (min - rayOrigin) * invDir;
    Vector3D tMaxVec = (max - rayOrigin) * invDir;

    Vector3D tEnter = Vector3D::Min(tMinVec, tMaxVec);
    Vector3D tExit = Vector3D::Max(tMinVec, tMaxVec);

    float tMin = std::max({ tEnter.x, tEnter.y, tEnter.z });
    float tMax = std::min({ tExit.x, tExit.y, tExit.z });

    // Vérification des intersections
    if (tMax < 0 || tMin > tMax || tMin > 1.0f || tMax < 0.0f)
    {
        return false;
    }

    outDistance = tMin;
    return true;
}
