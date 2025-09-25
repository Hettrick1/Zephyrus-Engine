#include "CubeColliderComponent.h"

#include "BulletRigidbodyComponent.h"
#include "Actor.h"
#include "SceneManager.h"

CubeColliderComponent::CubeColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner, "CubeColliderComponent")
{
    mShape = new btBoxShape(mHalfExtents.ToBulletVec3());
    mAppliedHalfExtents = mHalfExtents;
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
    pWriter.WriteVector3D("halfExtents", mHalfExtents);
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
    SetHalfExtents(mHalfExtents);
    SetIsQuery(mIsQuery);
    return
    {
        {"Is Querry : ", &mIsQuery, PropertyType::Bool},
        {"Ignore Self : ",&mIgnoreSelf, PropertyType::Bool},
        {"Half Extents : ", &mHalfExtents, PropertyType::Vec3}
    };
}

void CubeColliderComponent::SetHalfExtents(const Vector3D& pHalfExtents)
{
    if (pHalfExtents != mAppliedHalfExtents)
    {
        mHalfExtents = pHalfExtents;
        mAppliedHalfExtents = mHalfExtents;
        if (mGhost)
        {
            delete mShape;
            mShape = new btBoxShape(mHalfExtents.ToBulletVec3());
            RebuildCollider();
            return;
        }
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btBoxShape(mHalfExtents.ToBulletVec3());

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
