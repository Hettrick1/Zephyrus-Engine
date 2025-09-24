#pragma once
#include "BulletColliderComponent.h"

class CapsuleColliderComponent : public BulletColliderComponent
{
private:
    float mRadius = 1;
    float mHeight = 2;
public:
    CapsuleColliderComponent(Actor* pOwner);

    void Deserialize(const rapidjson::Value& pData) override;
    void Serialize(Serialization::Json::JsonWriter& pWriter) override;

    void OnStart() override;
    void OnEnd() override;

    static Component* Create(Actor* pOwner) { return new CapsuleColliderComponent(pOwner); }

    std::vector<PropertyDescriptor> GetProperties();

    void SetRadiusAndHeight(const float& pRadius = 1, const float& pHeight = 2);
};