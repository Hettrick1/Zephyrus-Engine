#include "CameraComponent.h"
#include "Actor.h"
#include "Vector3D.h"
#include "Log.h"
#include "Matrix4DRow.h"
#include "CameraManager.h"
#include "Scene.h"

CameraComponent::CameraComponent(Actor* pOwner, int updateOder)
    : Component(pOwner, updateOder)
{
	CameraManager::Instance().AddCamera(this);
}

CameraComponent::~CameraComponent()
{
}

Matrix4DRow CameraComponent::GetWorldTransform()
{
    if (mOwner)
    {
        return mRelativeTransform * mOwner->GetTransformComponent().GetWorldTransform();
    }
    return mRelativeTransform;
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
    Vector3D camPosition = mOwner->GetTransformComponent().GetPosition() + GetRelativePosition();
    Vector3D forward = worldTransform.GetYAxis();
    Vector3D target = camPosition + forward * 400.0f;
    Vector3D up = worldTransform.GetZAxis(); 

    Matrix4DRow view = Matrix4DRow::CreateLookAt(camPosition, target, up);
    mOwner->GetScene().GetRenderer()->SetViewMatrix(view);
}

