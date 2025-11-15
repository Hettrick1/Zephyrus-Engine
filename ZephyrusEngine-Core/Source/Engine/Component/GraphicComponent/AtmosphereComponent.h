#pragma once

#include "Component.h"

namespace Zephyrus::ActorComponent
{
    class AtmosphereComponent : Component
    {
    private:
        float mFogStart = 50.0f;
        float mFogEnd   = 200.0f;
        Vector4D mFogColor = Vector4D(0.6f, 0.7f, 0.9f, 1.0f);
    public:
        AtmosphereComponent(Actor* pOwner);

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        static Component* Create(Actor* pOwner) { return new AtmosphereComponent(pOwner); }

        void OnStart() override;
        void OnEnd() override;
        std::vector<PropertyDescriptor> GetProperties() override;

        inline float GetFogStart() const { return mFogStart; }
        inline float GetFogEnd() const { return mFogEnd; }
        inline Vector3D GetFogColor() const { return mFogColor.xyz; }
    };
}