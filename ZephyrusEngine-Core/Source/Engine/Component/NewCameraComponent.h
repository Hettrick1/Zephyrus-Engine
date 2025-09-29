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
private:
    Matrix4DRow mViewMatrix;
    Matrix4DRow mProjMatrix;

    float mFov = 70.0f;
    float mWidth = 1920;
    float mHeight = 1080;
    float mNearClip = 0.1f;
    float mFarClip = 10000.0f;
public:
    NewCameraComponent(Actor* pOwner, int pWidth = 1920, int pHeight = 1080, CameraUsage pUsage = CameraUsage::Game);
    ~NewCameraComponent();

    void Deserialize(const rapidjson::Value& pData) override;
    void Serialize(Serialization::Json::JsonWriter& pWriter) override;

    static Component* Create(Actor* pOwner) { return new NewCameraComponent(pOwner); }

    inline Matrix4DRow GetViewMatrix() const { return mViewMatrix; }
    inline Matrix4DRow GetProjMatrix() const { return mProjMatrix; }

    void SetDimensions(const Vector2D& pDimensions);
    void SetFOV(float pFov);
    void SetClipping(float pNearPlane, float pFarPlane);

    void UpdateMatrices();

    void RenderScene();

    CameraUsage usage;

    RenderTarget* renderTarget = nullptr;
};
