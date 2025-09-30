#include "NewCameraComponent.h"
#include "SceneManager.h"
#include "Log.h"
#include <glew.h>

CameraComponent::CameraComponent(Actor* pOwner, int pWidth, int pHeight, CameraUsage pUsage)
    : Component(pOwner, "CameraComponent"), usage(pUsage), mWidth(pWidth), mHeight(pHeight)
{
    renderTarget = new RenderTarget(pWidth, pHeight);
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    if (pUsage == CameraUsage::Game)
    {
        SceneManager::ActiveScene->GetCameraManager()->AddCamera(this);
    }
}

CameraComponent::~CameraComponent()
{
    if (renderTarget) {
        delete renderTarget;
        renderTarget = nullptr;
    }
}

void CameraComponent::Deserialize(const rapidjson::Value& pData)
{
    Component::Deserialize(pData);
}

void CameraComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    Component::Serialize(pWriter);
}

void CameraComponent::SetDimensions(const Vector2D& pDimensions)
{
    if (mWidth == pDimensions.x && mHeight == pDimensions.y)
    {
        return;
    }
    mWidth = pDimensions.x;
    mHeight = pDimensions.y;
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    renderTarget->Resize(mWidth, mHeight);
    SceneManager::ActiveScene->GetRenderer()->SetProjMatrix(mProjMatrix);
}

inline void CameraComponent::SetFOV(float pFov)
{
    mFov = pFov;
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    SceneManager::ActiveScene->GetRenderer()->SetProjMatrix(mProjMatrix);
}

inline void CameraComponent::SetClipping(float pNearPlane, float pFarPlane)
{
    mNearClip = pNearPlane;
    mFarClip = pFarPlane;
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    SceneManager::ActiveScene->GetRenderer()->SetProjMatrix(mProjMatrix);
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
    SceneManager::ActiveScene->GetRenderer()->SetViewMatrix(mViewMatrix);
}

void CameraComponent::RenderScene()
{
    if (!renderTarget) return;

    renderTarget->Bind();
    glViewport(0, 0, renderTarget->GetDimensions().x, renderTarget->GetDimensions().y);
    SceneManager::ActiveScene->BeginRender();
    SceneManager::ActiveScene->RenderCurrentSceneOnly();
    auto world = SceneManager::ActiveScene->GetPhysicWorld();
    world->GetWorld()->debugDrawWorld();
    auto debugRenderer = SceneManager::ActiveScene->GetPhysicDebugRenderer();
    debugRenderer->FlushDraw(this);
    SceneManager::ActiveScene->EndRender();

    RenderTarget::Unbind();
}