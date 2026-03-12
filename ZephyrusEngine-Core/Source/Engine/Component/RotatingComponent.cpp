#include "pch.h"
#include "RotatingComponent.h"

#include "Timer.h"

namespace Zephyrus::ActorComponent
{
    RotatingComponent::RotatingComponent(Actor* pOwner)
        : Component(pOwner, "RotatingComponent")
    {
    }

    RotatingComponent::~RotatingComponent()
    {
    }

    void RotatingComponent::Deserialize(Serialization::IDeserializer& pReader)
    {
        Component::Deserialize(pReader);
        if (auto rotationRate = pReader.ReadVector3D("rotationRate"))
        {
            mRotationRate = *rotationRate;
        }
    }

    void RotatingComponent::Serialize(Serialization::ISerializer& pWriter)
    {
        Component::BeginSerialize(pWriter);
        pWriter.WriteVector3D("rotationRate", mRotationRate);
        Component::EndSerialize(pWriter);
    }

    std::vector<PropertyDescriptor> RotatingComponent::GetProperties()
    {
        return{
            { "Rotation Rate : ", &mRotationRate, PropertyType::Vec3 }
        };
    }

    void RotatingComponent::OnStart()
    {
        Component::OnStart();
    }

    void RotatingComponent::Update()
    {
        Component::Update();
        mOwner->RotateZ(mRotationRate.z * Timer::deltaTime);
        mOwner->RotateY(mRotationRate.y * Timer::deltaTime);
        mOwner->RotateX(mRotationRate.x * Timer::deltaTime);
    }

    void RotatingComponent::OnEnd()
    {
        Component::OnEnd();
    }
}
