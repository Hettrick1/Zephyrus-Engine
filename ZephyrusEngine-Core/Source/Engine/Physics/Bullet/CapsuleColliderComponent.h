#pragma once
#include "BulletColliderComponent.h"

namespace Zephyrus::ActorComponent {
    class CapsuleColliderComponent : public BulletColliderComponent
    {
    private:
        float mRadius = 1;
        float mHeight = 2;
        float mAppliedRadius = -1;
        float mAppliedHeight = -2;
    public:
        CapsuleColliderComponent(Actor* pOwner);

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        void OnStart() override;
        void OnEnd() override;

        static Component* Create(Actor* pOwner) { return new CapsuleColliderComponent(pOwner); }

        std::vector<PropertyDescriptor> GetProperties();

        void SetRadiusAndHeight(const float& pRadius = 1, const float& pHeight = 2);
    };
}