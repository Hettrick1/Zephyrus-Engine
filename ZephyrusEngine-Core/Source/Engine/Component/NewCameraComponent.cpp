#include "NewCameraComponent.h"
#include "SceneManager.h"
#include "Log.h"
#include <glew.h>

NewCameraComponent::NewCameraComponent(Actor* pOwner, int pWidth, int pHeight, CameraUsage pUsage)
    : Component(pOwner, "NewCameraComponent"), usage(pUsage), mWidth(pWidth), mHeight(pHeight)
{
    renderTarget = new RenderTarget(pWidth, pHeight);
    if (pUsage == CameraUsage::Game)
    {
        SceneManager::ActiveScene->GetCameraManager()->AddCamera(this);
    }
}

NewCameraComponent::~NewCameraComponent()
{
    if (renderTarget) {
        delete renderTarget;
        renderTarget = nullptr;
    }
}

void NewCameraComponent::Deserialize(const rapidjson::Value& pData)
{
    Component::Deserialize(pData);
}

void NewCameraComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    Component::Serialize(pWriter);
}

void NewCameraComponent::SetDimensions(const Vector2D& pDimensions)
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

inline void NewCameraComponent::SetFOV(float pFov)
{
    mFov = pFov;
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    SceneManager::ActiveScene->GetRenderer()->SetProjMatrix(mProjMatrix);
}

inline void NewCameraComponent::SetClipping(float pNearPlane, float pFarPlane)
{
    mNearClip = pNearPlane;
    mFarClip = pFarPlane;
    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
    SceneManager::ActiveScene->GetRenderer()->SetProjMatrix(mProjMatrix);
}

void NewCameraComponent::UpdateMatrices()
{
    if (!mOwner) {
        ZP_CORE_ERROR("NewCameraComponent::UpdateMatrices: Owner actor is null!");
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

void NewCameraComponent::RenderScene()
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