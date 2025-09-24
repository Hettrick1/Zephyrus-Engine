#include "SphereColliderComponent.h"
#include "Actor.h"
#include "BulletRigidbodyComponent.h"

SphereColliderComponent::SphereColliderComponent(Actor* pOwner)
    : BulletColliderComponent(pOwner, "SphereColliderComponent")
{
	mShape = new btSphereShape(mRadius);
    if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
    {
        rb->AddCollider(this);
    }
    else
    {
        CreateColliderWithoutBody();
    }
}

void SphereColliderComponent::Deserialize(const rapidjson::Value& pData)
{
    BulletColliderComponent::Deserialize(pData);
    if (auto radius = Serialization::Json::ReadFloat(pData, "radius"))
    {
        SetRadius(*radius);
    }
}

void SphereColliderComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
    BulletColliderComponent::BeginSerialize(pWriter);
    pWriter.WriteFloat("radius", mRadius);
    BulletColliderComponent::EndSerialize(pWriter);
}

void SphereColliderComponent::OnStart()
{
}

void SphereColliderComponent::OnEnd()
{
}

std::vector<PropertyDescriptor> SphereColliderComponent::GetProperties()
{
    SetRadius(mRadius);
    SetIsQuery(mIsQuery);
    return
    {
        {"Is Querry : ", &mIsQuery, PropertyType::Bool},
        {"Ignore Self : ",&mIgnoreSelf, PropertyType::Bool},
        {"Radius : ", &mRadius, PropertyType::Float}
    };
}

void SphereColliderComponent::SetRadius(const float& pRadius)
{
    float newRadius = pRadius;
    if (newRadius != mRadius)
    {
        mRadius = newRadius;
        if (mShape)
        {
            btCollisionShape* oldShape = mShape;

            mShape = new btSphereShape(mRadius);

            if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
            {
                rb->UpdateColliderShape(this, oldShape);
            }

            delete oldShape;
        }
    }
}
