#pragma once
#include "RenderTarget.h"
#include "Matrix4DRow.h"
#include "Vector3D.h"
#include "Component.h"
#include "Actor.h"

enum class CameraUsage 
{ 
    Game, 
    Editor 
};

class NewCameraComponent : public Component
{
public:
    NewCameraComponent(Actor* pOwner, int pWidth, int pHeight, CameraUsage pUsage = CameraUsage::Game);

    ~NewCameraComponent();

    inline Matrix4DRow GetViewMatrix() const { return mViewMatrix; }
    inline Matrix4DRow GetProjMatrix() const { return mProjMatrix; }

    inline void SetDimensions(const Vector2D& pDimensions) 
    {
        mWidth = pDimensions.x;
        mHeight = pDimensions.y;
    }
    inline void SetFOV(float pFov) { mFov = pFov; }
    inline void SetClipping(float pNearPlane, float pFarPlane) 
    {
        mNearClip = pNearPlane;
        mFarClip = pFarPlane;
    }

    void UpdateMatrices();

    void RenderScene();

    CameraUsage usage;

    RenderTarget* renderTarget = nullptr;
private:
    Matrix4DRow mViewMatrix;
    Matrix4DRow mProjMatrix;

    float mFov = 60.0f;
    float mWidth = 1920;
    float mHeight = 1080;
    float mNearClip = 0.00001f;
    float mFarClip = 1000000.0f;
};
