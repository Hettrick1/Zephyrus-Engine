#pragma once

namespace Zephyrus::ActorComponent
{
    class Actor;
}

class SelectedActorAccessor
{
    public:
    virtual ~SelectedActorAccessor() = default;
    virtual Zephyrus::ActorComponent::Actor* GetSelectedActor() const = 0;
};
