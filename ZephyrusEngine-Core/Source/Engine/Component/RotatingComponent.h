#pragma once

#include "Component.h"
#include "IRenderer.h"
#include "Interface/IMesh.h"

namespace Zephyrus::ActorComponent
{
    class RotatingComponent : public Component
    {
    private:
        Vector3D mRotationRate{1.0f, 1.0f, 1.0f};    
    public:
        RotatingComponent(Actor* pOwner);
        virtual ~RotatingComponent() override;

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;
        
        static Component* Create(Actor* pOwner) { return new RotatingComponent(pOwner); }
        virtual std::vector<PropertyDescriptor> GetProperties() override;

        void OnStart() override;
        void Update() override;
        void OnEnd() override;
    };
}
