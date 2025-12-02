#include "pch.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "Log.h"
#include <glew.h>

using Zephyrus::Scenes::SceneManager;

namespace Zephyrus::ActorComponent
{
    CameraComponent::CameraComponent(Actor* pOwner, int pWidth, int pHeight, CameraUsage pUsage)
        : Component(pOwner, "CameraComponent"), usage(pUsage), mWidth(pWidth), mHeight(pHeight)
    {
        mRenderTarget = new Zephyrus::Render::RenderTargetOpenGL(pWidth, pHeight);
        mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
        if (pUsage == CameraUsage::Game)
        {
            mOwner->GetSceneContext()->GetCameraManager()->AddCamera(this);
        }
    }

    CameraComponent::~CameraComponent()
    {
        if (mRenderTarget) {
            delete mRenderTarget;
            mRenderTarget = nullptr;
        }
    }

    void CameraComponent::Deserialize(Serialization::IDeserializer& pReader)
    {
        Component::Deserialize(pReader);
        if (auto fov = pReader.ReadFloat("fov"))
        {
            mFov = *fov;
        }
        if (auto width = pReader.ReadFloat("width"))
        {
            mWidth = *width;
        }
        if (auto height = pReader.ReadFloat("height"))
        {
            mHeight = *height;
        }
        if (auto nearClip = pReader.ReadFloat("nearClip"))
        {
            mNearClip = *nearClip;
        }
        if (auto farClip = pReader.ReadFloat("farClip"))
        {
            mFarClip = *farClip;
        }
        SetFov(mFov);
        SetDimensions(Vector2D(mWidth, mHeight));
        SetClipping(mNearClip, mFarClip);
    }

    void CameraComponent::Serialize(Serialization::ISerializer& pWriter)
    {
        Component::BeginSerialize(pWriter);
        pWriter.WriteFloat("fov", mFov);
        pWriter.WriteFloat("width", mWidth);
        pWriter.WriteFloat("height", mHeight);
        pWriter.WriteFloat("nearClip", mNearClip);
        pWriter.WriteFloat("farClip", mFarClip);
        Component::EndSerialize(pWriter);
    }

    std::vector<PropertyDescriptor> CameraComponent::GetProperties()
    {
        SetFov(mFov);
        SetDimensions(Vector2D(mWidth, mHeight));
        SetClipping(mNearClip, mFarClip);
        return
        {
            {"Fov : ", &mFov, PropertyType::Float, Zephyrus::Range(40.0f, 120.0f)},
            {"Width : ", &mWidth, PropertyType::Float},
            {"Height : ", &mHeight, PropertyType::Float},
            {"NearClip : ", &mNearClip, PropertyType::Float},
            {"FarClip : ", &mFarClip, PropertyType::Float},
        };
    }

    void CameraComponent::SetDimensions(const Vector2D& pDimensions)
    {
        if (mOldWidth == pDimensions.x && mOldHeight == pDimensions.y)
        {
            return;
        }
        mOldWidth = pDimensions.x;
        mOldHeight = pDimensions.y;
        mWidth = pDimensions.x;
        mHeight = pDimensions.y;
        mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
        mRenderTarget->Resize(mWidth, mHeight);
    }

    inline void CameraComponent::SetFov(float pFov)
    {
        if (mOldFov == pFov)
        {
            return;
        }
        if (pFov > 120)
        {
            mOldFov = 120;
            mFov = 120;
        }
        else if (pFov < 40)
        {
            mOldFov = 40;
            mFov = 40;
        }
        else
        {
            mOldFov = pFov;
            mFov = pFov;
        }
        mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    }

    inline void CameraComponent::SetClipping(float pNearPlane, float pFarPlane)
    {
        if (mOldNear == pNearPlane && mOldFar == pFarPlane)
        {
            return;
        }
        mOldNear = pNearPlane;
        mOldFar = pFarPlane;
        mNearClip = pNearPlane;
        mFarClip = pFarPlane;
        mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    }

    void CameraComponent::UpdateMatrices()
    {
        if (!mOwner) {
            ZP_CORE_ERROR("CameraComponent::UpdateMatrices: Owner actor is null!");
            return;
        }

        Matrix4DRow worldTransform = GetWorldTransform();
        Vector3D camPosition = worldTransform.GetTranslation();
        Vector3D forward = worldTransform.GetYAxis();
        Vector3D target = camPosition + forward * 400.0f;
        Vector3D up = worldTransform.GetZAxis();

        Matrix4DRow view = Matrix4DRow::CreateLookAt(camPosition, target, up);
        mViewMatrix = view;
    }

    void CameraComponent::RenderScene()
    {
        if (!mRenderTarget) return;

        mRenderTarget->Bind();

        mOwner->GetSceneContext()->GetRenderer()->SetProjMatrix(mProjMatrix);
        mOwner->GetSceneContext()->GetRenderer()->SetViewMatrix(mViewMatrix);
        mOwner->GetSceneContext()->GetRenderer()->SetCameraPosition(GetWorldPosition());

        mOwner->GetSceneContext()->GetActiveScene()->BeginRender();
        mOwner->GetSceneContext()->GetActiveScene()->RenderCurrentSceneOnly();
        // // TODO move debug draw for physics
        // auto world = mOwner->GetSceneContext()->GetPhysicsWorld();
        // world->GetWorld()->debugDrawWorld();
        // auto debugRenderer = mOwner->GetSceneContext()->GetActiveScene()->GetPhysicDebugRenderer();
        // debugRenderer->FlushDraw(this);

        mRenderTarget->Unbind();
    }
}