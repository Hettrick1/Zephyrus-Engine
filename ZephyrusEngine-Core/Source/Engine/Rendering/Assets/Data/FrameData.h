#pragma once

#include "Vector3D.h"
#include  "Vector2D.h"

namespace Zephyrus::Render
{
    struct FrameData
    {
        FrameData():
        cameraPos(Vector3D(0.0f, 0.0f, 0.0f)),
        time(0.0f),
        screenWidth(1920.0f),
        screenHeight(1920.0f),
        fogStart(50.0f),
        fogEnd(200.0f),
        fogColor(Vector3D(0.0f, 0.0f, 0.0f))
        {
            
        }
        
        Vector3D cameraPos;

        float time;        
        float screenWidth;
        float screenHeight;
        float fogStart;    
        float fogEnd;      

        Vector3D fogColor; 
    };
}