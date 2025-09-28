#include "CameraComponent.h"
#include "Actor.h"
#include "Vector3D.h"
#include "Log.h"
#include "Matrix4DRow.h"
#include "CameraManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ComponentFactory.h"

CameraComponent::CameraComponent(Actor* pOwner, int updateOder)
    : Component(pOwner, "CameraComponent", updateOder)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Deserialize(const rapidjson::Value& pData)
{
    Component::Deserialize(pData);
}

void CameraComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    Component::Serialize(pWriter);
}

void CameraComponent::OnStart()
{
    Component::OnStart();
    CameraManager::Instance().AddCamera(this);
}

void CameraComponent::OnEnd()
{
    CameraManager::Instance().RemoveCamera(this);
    Component::OnEnd();
}

void CameraComponent::ComputeRelativeTransform()
{
    if (mOwner)
    {
        mRelativeTransform = Matrix4DRow::CreateScale(mRelativeSize);
        mRelativeTransform *= Matrix4DRow::CreateFromQuaternion(mRelativeRotation);
        mRelativeTransform *= Matrix4DRow::CreateTranslation(mRelativePosition);
    }
    else
    {
        mRelativeTransform = Matrix4DRow::Identity;
    }
}

void CameraComponent::UpdateCam()
{
    Matrix4DRow worldTransform = GetWorldTransform();
    Vector3D camPosition = GetWorldTransform().GetTranslation();
    Vector3D forward = worldTransform.GetYAxis();
    Vector3D target = camPosition + forward * 400.0f;
    Vector3D up = worldTransform.GetZAxis(); 

    Matrix4DRow view = Matrix4DRow::CreateLookAt(camPosition, target, up);
    mViewMatrix = view;
    SceneManager::ActiveScene->GetRenderer()->SetViewMatrix(view);
}

