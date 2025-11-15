#pragma once

#include "Vector3D.h"
#include  "Vector2D.h"

namespace Zephyrus::Render
{
    struct FrameData
    {
        Vector3D cameraPos; // 12 bytes

        float time;         // 4 bytes
        float screenWidth;
        float screenHeight;
        float fogStart;     // 4 bytes
        float fogEnd;       // 4 bytes

        Vector3D fogColor;  // 12 bytes
    };
}