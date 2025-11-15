#include "AtmosphereComponent.h"

#include "Actor.h"
#include "IRenderer.h"

namespace Zephyrus::ActorComponent
{
    AtmosphereComponent::AtmosphereComponent(Actor* pOwner)
        :Component(pOwner, "AtmosphereComponent")
    {
    }

    void AtmosphereComponent::Deserialize(Serialization::IDeserializer& pReader)
    {
        Component::Deserialize(pReader);
        if (auto start = pReader.ReadFloat("fogStart"))
        {
            mFogStart = start.value();
        }
        if (auto end = pReader.ReadFloat("fogEnd"))
        {
            mFogEnd = end.value();
        }
        if (auto color = pReader.ReadVector4D("fogColor"))
        {
            mFogColor = color.value();
        }
    }

    void AtmosphereComponent::Serialize(Serialization::ISerializer& pWriter)
    {
        Component::BeginSerialize(pWriter);
        pWriter.WriteFloat("fogStart", mFogStart);
        pWriter.WriteFloat("fogEnd", mFogEnd);
        pWriter.WriteVector4D("fogColor", mFogColor);
        Component::EndSerialize(pWriter);
    }

    void AtmosphereComponent::OnStart()
    {
        Component::OnStart();
        mOwner->GetSceneContext()->GetRenderer()->SetAtmosphereComponent(this);
    }

    void AtmosphereComponent::OnEnd()
    {
        Component::OnEnd();
        mOwner->GetSceneContext()->GetRenderer()->SetAtmosphereComponent(nullptr);
    }

    std::vector<PropertyDescriptor> AtmosphereComponent::GetProperties()
    {
        return
        {
			    { "Fog Start : ", &mFogStart, PropertyType::Float },
                { "Fog End : ", &mFogEnd, PropertyType::Float },
                { "Fog Color : ", &mFogColor, PropertyType::Color },
        };
    }
}
