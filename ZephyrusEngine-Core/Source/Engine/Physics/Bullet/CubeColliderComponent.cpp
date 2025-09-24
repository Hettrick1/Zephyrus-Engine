#include "CubeColliderComponent.h"

#include "BulletRigidbodyComponent.h"
#include "Actor.h"
#include "SceneManager.h"

CubeColliderComponent::CubeColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner)
{
    mShape = new btBoxShape(mHalfExtents);
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
    else
    {
        CreateColliderWithoutBody();
    }
}

void CubeColliderComponent::Deserialize(const rapidjson::Value& pData)
{
    BulletColliderComponent::Deserialize(pData);
    if (auto halfExtents = Serialization::Json::ReadVector3D(pData, "halfExtents"))
    {
        SetHalfExtents(*halfExtents);
    }
}

void CubeColliderComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    BulletColliderComponent::BeginSerialize(pWriter);
    pWriter.WriteVector3D("halfExtents", Vector3D(mHalfExtents.x(), mHalfExtents.y(), mHalfExtents.z()));
    BulletColliderComponent::EndSerialize(pWriter);
}

void CubeColliderComponent::OnStart()
{
}

void CubeColliderComponent::OnEnd()
{
}

std::vector<PropertyDescriptor> CubeColliderComponent::GetProperties()
{
    return std::vector<PropertyDescriptor>();
}

void CubeColliderComponent::SetHalfExtents(const Vector3D& pHalfExtents)
{
    btVector3 newExtents = pHalfExtents.ToBulletVec3();
    if (newExtents != mHalfExtents)
    {
        mHalfExtents = newExtents;
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btBoxShape(mHalfExtents);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
