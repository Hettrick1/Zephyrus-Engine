#pragma once
#include "BulletColliderComponent.h"

class SphereColliderComponent : public BulletColliderComponent
{
private:
    float mRadius = 1;
    float mAppliedRadius = -1.0f;
public:
    SphereColliderComponent(Actor* pOwner);

    void Deserialize(const rapidjson::Value& pData) override;
    void Serialize(Serialization::Json::JsonWriter& pWriter) override;

    void OnStart() override;
    void OnEnd() override;

    static Component* Create(Actor* pOwner) { return new SphereColliderComponent(pOwner); }

    std::vector<PropertyDescriptor> GetProperties();

    void SetRadius(const float& pRadius);
};
