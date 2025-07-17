#include "TransformComponent.h"
#include "Actor.h"
#include "Log.h"

TransformComponent::TransformComponent(Actor* pOwner, Vector3D position, Vector3D size, Quaternion rotation)
    : mOwner(pOwner), mPosition(position), mSize(size), mRotation(rotation)
{
}

TransformComponent::TransformComponent()
    : mPosition(0), mSize(1), mRotation(0, 0)
{

}

Vector3D TransformComponent::GetPosition()
{
    return mPosition;
}

Vector3D TransformComponent::GetSize()
{
    return mSize;
}

Quaternion TransformComponent::GetRotation()
{
    return mRotation;
}

Matrix4DRow TransformComponent::GetWorldTransform()
{
    return mWorldTransform;
}

void TransformComponent::SetPosition(Vector3D newPosition)
{
    mPosition = newPosition;
    ComputeWorldTransform();
}

void TransformComponent::Translate(Vector3D pTranslation)
{
    mPosition += pTranslation;
    ComputeWorldTransform();
}

void TransformComponent::SetSize(Vector3D newSize)
{
    mSize = newSize;
    ComputeWorldTransform();
}

void TransformComponent::SetRotation(Quaternion newRotation)
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
    float piAngle = Maths::ToRad(pAngle);
    mPitch += pAngle;
    Quaternion newX(Vector3D::unitX, piAngle);
    mRotation = Quaternion::Concatenate(newX, mRotation);
    /*Log::Info("x : " + std::to_string(Maths::ToDeg(mRotation.x)));
    Log::Info("y : " + std::to_string(Maths::ToDeg(mRotation.y)));
    Log::Info("z : " + std::to_string(Maths::ToDeg(mRotation.z)));*/
    ComputeWorldTransform();
}

void TransformComponent::RotateY(float pAngle)
{
    float piAngle = Maths::ToRad(pAngle);
    mRoll += pAngle;
    Quaternion newY(Vector3D::unitY, piAngle);
    mRotation = Quaternion::Concatenate(newY, mRotation);
    /*Log::Info("x : " + std::to_string(Maths::ToDeg(mRotation.x)));
    Log::Info("y : " + std::to_string(Maths::ToDeg(mRotation.y)));
    Log::Info("z : " + std::to_string(Maths::ToDeg(mRotation.z)));*/
    ComputeWorldTransform();
} 

void TransformComponent::RotateZ(float pAngle)
{
    float piAngle = Maths::ToRad(pAngle);
    mYaw += pAngle;
    Quaternion newZ(Vector3D::unitZ, piAngle);
    mRotation = Quaternion::Concatenate(newZ, mRotation);
    /*Log::Info("x : " + std::to_string(Maths::ToDeg(mRotation.x)));
    Log::Info("y : " + std::to_string(Maths::ToDeg(mRotation.y)));
    Log::Info("z : " + std::to_string(Maths::ToDeg(mRotation.z)));*/
    ComputeWorldTransform(); 
}

void TransformComponent::ComputeWorldTransform()
{
    mWorldTransform = Matrix4DRow::CreateScale(mSize);
    mWorldTransform *= Matrix4DRow::CreateFromQuaternion(mRotation);
    mWorldTransform *= Matrix4DRow::CreateTranslation(mPosition);
    mOwner->UpdateComponentsTransform();
}


