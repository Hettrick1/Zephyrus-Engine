#include "Component.h"
#include "Actor.h"
#include "Log.h"
#include "FactoryJSON/JSONUtils.h"

Component::Component(Actor* pOwner, int pUpdateOder) 
    : mOwner(pOwner), mUpdateOrder(pUpdateOder), mRelativePosition(0),
    mRelativeRotation(Quaternion(0, 0, 0, 1)), mRelativeSize(1)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
}

void Component::OnStart()
{
}

void Component::Update()
{
}

void Component::OnEnd()
{
    mOwner = nullptr;
}

void Component::Deserialize(const rapidjson::Value& pData)
{
    if (auto pos = ReadVector3D(pData, "relativePosition"))
    {
        SetRelativePosition(*pos);
    }

    if (auto size = ReadVector3D(pData, "relativeSize"))
    {
        SetRelativeSize(*size);
    }

    if (auto rot = ReadVector3D(pData, "relativeRotation"))
    {
        SetRelativeRotation(Quaternion(*rot));
    }
}

void Component::SetRelativePosition(const Vector3D& pPosition)
{
    mRelativePosition = pPosition;
    ComputeRelativeTransform();
}

void Component::SetRelativeSize(const Vector3D& pSize)
{
    mRelativeSize = pSize;
    ComputeRelativeTransform();
}

void Component::SetRelativeRotation(const Quaternion& pRotation)
{
    mRelativeRotation = pRotation;
    ComputeRelativeTransform();
}

void Component::RelativeRotateX(float pAngle)
{
    float piAngle = Maths::ToRad(pAngle);
    Quaternion newX(Vector3D::unitX, piAngle);
    mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
    ComputeRelativeTransform();
}

void Component::RelativeRotateY(float pAngle)
{
    float piAngle = Maths::ToRad(pAngle);
    Quaternion newX(Vector3D::unitY, piAngle);
    mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
    ComputeRelativeTransform();
}

void Component::RelativeRotateZ(float pAngle)
{
    float piAngle = Maths::ToRad(pAngle);
    Quaternion newX(Vector3D::unitZ, piAngle);
    mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
    ComputeRelativeTransform();
}

Matrix4DRow Component::GetWorldTransform()
{
    if (mOwner)
    {
        return mRelativeTransform * mOwner->GetTransformComponent().GetWorldTransform();
    }
    return mRelativeTransform;
}

void Component::ComputeRelativeTransform()
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

Actor* Component::GetOwner() const
{
    return mOwner;
}

Vector3D Component::GetWorldPosition() const
{
    return mOwner->GetTransformComponent().GetPosition() + mRelativePosition;
}
