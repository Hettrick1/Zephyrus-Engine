#pragma once
#include "BulletColliderComponent.h"

namespace Zephyrus::ActorComponent {
    class SphereColliderComponent : public BulletColliderComponent
    {
    private:
        float mRadius = 1;
        float mAppliedRadius = -1.0f;
    public:
        SphereColliderComponent(Actor* pOwner);

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        void OnStart() override;
        void OnEnd() override;

        static Component* Create(Actor* pOwner) { return new SphereColliderComponent(pOwner); }

        std::vector<PropertyDescriptor> GetProperties();

        void SetRadius(const float& pRadius);
    };
}
