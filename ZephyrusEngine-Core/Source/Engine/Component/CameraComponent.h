#pragma once
#include "RenderTarget.h"
#include "Matrix4DRow.h"
#include "Vector3D.h"
#include "Component.h"
#include "Actor.h"

namespace Zephyrus::ActorComponent
{
    enum class CameraUsage
    {
        Game,
        Editor
    };

    class CameraComponent : public Component
    {
    private:
        Matrix4DRow mViewMatrix;
        Matrix4DRow mProjMatrix;

        float mFov = 70.0f;
        float mOldFov = mFov;
        float mWidth = 1920;
        float mOldWidth = mWidth;
        float mHeight = 1080;
        float mOldHeight = mHeight;
        float mNearClip = 0.1f;
        float mOldNear = mNearClip;
        float mFarClip = 10000.0f;
        float mOldFar = mFarClip;
    public:
        GLsync mFence = 0;

        CameraComponent(Actor* pOwner, int pWidth = 1920, int pHeight = 1080, CameraUsage pUsage = CameraUsage::Game);
        ~CameraComponent();

        void Deserialize(const rapidjson::Value& pData) override;
        void Serialize(Serialization::Json::JsonWriter& pWriter) override;

        static Component* Create(Actor* pOwner) { return new CameraComponent(pOwner); }

        std::vector<PropertyDescriptor> GetProperties() override;

        inline Matrix4DRow GetViewMatrix() const { return mViewMatrix; }
        inline Matrix4DRow GetProjMatrix() const { return mProjMatrix; }

        void SetDimensions(const Vector2D& pDimensions);
        void SetFov(float pFov);
        void SetClipping(float pNearPlane, float pFarPlane);

        void UpdateMatrices();

        void RenderScene();

        CameraUsage usage;

        RenderTarget* renderTarget = nullptr;
    };
}
