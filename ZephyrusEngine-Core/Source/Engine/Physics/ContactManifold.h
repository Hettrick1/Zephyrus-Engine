#pragma once

#include "Vector3D.h"
#include <vector>

struct ContactManifold
{
    Vector3D normal;
    float penetrationDepth;
    std::vector<Vector3D> contactPoints;
};