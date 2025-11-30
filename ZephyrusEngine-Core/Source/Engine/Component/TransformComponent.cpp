#include "pch.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "Log.h"

namespace Zephyrus::ActorComponent
{
    TransformComponent::TransformComponent(Actor* pOwner, const Vector3D& pPosition, const Vector3D& pSize, const Quaternion& pRotation)
        : mOwner(pOwner), mPosition(pPosition), mSize(pSize), mRotation(pRotation)
    {
    }

    TransformComponent::TransformComponent()
        : mPosition(0), mSize(1), mRotation(0, 0)
    {

    }

    void TransformComponent::SetPosition(const Vector3D& newPosition)
    {
        mPosition = newPosition;
        ComputeWorldTransform();
    }

    void TransformComponent::Translate(const Vector3D& pTranslation)
    {
        mPosition += pTranslation;
        ComputeWorldTransform();
    }

    void TransformComponent::SetSize(const Vector3D& newSize)
    {
        mSize = newSize;
        ComputeWorldTransform();
    }

    void TransformComponent::SetRotation(const Quaternion& newRotation)
    {
        mRotation = newRotation;
        ComputeWorldTransform();
    }

    void TransformComponent::SetOwner(Actor* pOwner)
    {
        mOwner = pOwner;
    }

    void TransformComponent::RotateX(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        mPitch += pAngle;
        Quaternion newX(Vector3D::unitX, piAngle);
        mRotation = Quaternion::Concatenate(newX, mRotation);
        ComputeWorldTransform();
    }

    void TransformComponent::RotateY(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        mRoll += pAngle;
        Quaternion newY(Vector3D::unitY, piAngle);
        mRotation = Quaternion::Concatenate(newY, mRotation);
        ComputeWorldTransform();
    }

    void TransformComponent::RotateZ(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        mYaw += pAngle;
        Quaternion newZ(Vector3D::unitZ, piAngle);
        mRotation = Quaternion::Concatenate(newZ, mRotation);
        ComputeWorldTransform();
    }

    void TransformComponent::ComputeWorldTransform()
    {
        mWorldTransform = Matrix4DRow::CreateScale(mSize);
        mWorldTransform *= Matrix4DRow::CreateFromQuaternion(mRotation);
        mWorldTransform *= Matrix4DRow::CreateTranslation(mPosition);
    }
}


