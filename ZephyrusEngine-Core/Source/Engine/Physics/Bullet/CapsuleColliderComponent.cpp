#include "CapsuleColliderComponent.h"
#include "Actor.h"
#include "BulletRigidbodyComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner, "CapsuleColliderComponent")
{
	mShape = new btCapsuleShapeZ(mRadius, mHeight);
    mAppliedRadius = mRadius;
    mAppliedHeight = mHeight;
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
    else
    {
        CreateColliderWithoutBody();
    }
}

void CapsuleColliderComponent::Deserialize(const rapidjson::Value& pData)
{
    BulletColliderComponent::Deserialize(pData);
    if (auto radius = Serialization::Json::ReadFloat(pData, "radius"))
    {
        mRadius = *radius;
    }
    if (auto height = Serialization::Json::ReadFloat(pData, "height"))
    {
        mHeight = *height;
    }
    SetRadiusAndHeight(mRadius, mHeight);
}

void CapsuleColliderComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    BulletColliderComponent::BeginSerialize(pWriter);
    BulletColliderComponent::Serialize(pWriter);
    pWriter.WriteFloat("radius", mRadius);
    pWriter.WriteFloat("height", mHeight);
    BulletColliderComponent::EndSerialize(pWriter);
}

void CapsuleColliderComponent::OnStart()
{
    BulletColliderComponent::OnStart();
}

void CapsuleColliderComponent::OnEnd()
{
}

std::vector<PropertyDescriptor> CapsuleColliderComponent::GetProperties()
{
    SetRadiusAndHeight(mRadius, mHeight);
    SetIsQuery(mIsQuery);
    return
    {
        {"Is Querry : ", &mIsQuery, PropertyType::Bool},
        {"Ignore Self : ",&mIgnoreSelf, PropertyType::Bool},
        {"Radius : ", &mRadius, PropertyType::Float},
        {"Height : ", &mHeight, PropertyType::Float}
    };
}

void CapsuleColliderComponent::SetRadiusAndHeight(const float& pRadius, const float& pHeight)
{
    if (!mIsActive)
    {
        return;
    }
    float newRadius = pRadius;
    float newHeight = pHeight;
    if (newRadius != mAppliedRadius || newHeight != mAppliedHeight)
    {
        mRadius = newRadius;
        mHeight = newHeight;
        mAppliedRadius = mRadius;
        mAppliedHeight = mHeight;
        if (mGhost)
        {
            delete mShape;
            mShape = new btCapsuleShapeZ(mRadius, mHeight);
            RebuildCollider();
            return;
        }
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btCapsuleShapeZ(mRadius, mHeight);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
