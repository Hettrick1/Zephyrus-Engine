#pragma once
#include "BulletColliderComponent.h"

class CubeColliderComponent : public BulletColliderComponent
{
private:
    Vector3D mHalfExtents = 1;
public:
    CubeColliderComponent(Actor* pOwner);

    void Deserialize(const rapidjson::Value& pData) override;
    void Serialize(Serialization::Json::JsonWriter& pWriter) override;

    void OnStart() override;
    void OnEnd() override;

    static Component* Create(Actor* pOwner) { return new CubeColliderComponent(pOwner); }

    std::vector<PropertyDescriptor> GetProperties();

    void SetHalfExtents(const Vector3D& pHalfExtents);
};
