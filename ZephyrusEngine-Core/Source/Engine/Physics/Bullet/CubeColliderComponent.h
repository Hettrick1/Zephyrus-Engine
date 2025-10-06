#pragma once
#include "BulletColliderComponent.h"

namespace Zephyrus::ActorComponent {
    class CubeColliderComponent : public BulletColliderComponent
    {
    private:
        Vector3D mHalfExtents = 1;
        Vector3D mAppliedHalfExtents = -1;
    public:
        CubeColliderComponent(Actor* pOwner);

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        void OnStart() override;
        void OnEnd() override;

        static Component* Create(Actor* pOwner) { return new CubeColliderComponent(pOwner); }

        std::vector<PropertyDescriptor> GetProperties();

        void SetHalfExtents(const Vector3D& pHalfExtents);
    };
}
